#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include "hashr.h" 
#include "networking.h" //Most defines for flags are in this header
#include "file_metadata.h"
#include "session_queue.h"

#define FAIL_BIND_SOCKET (-1)
#define COMMAND_ERROR 1
#define ARG_TOO_SMALL 1
#define POORLY_FORMED_ARGUMENT 2
#define SUCCESS 0
#define FAIL 1

//Global variable used to store file hash and verify integrity	
uint32_t hash_count = 0;
int hashes = 0;

int handshake_server(int s, uint32_t *buffer, struct queue *qp, int *session_id_list)
{
	int offset_major_byte = 1;
	int offset_minor_byte = 2;
	//Recv the session_start signal
	if (read_buffer(s, buffer, PACKET_BYTES) != PACKET_BYTES){
		return fatal_error_hangup(s, buffer, PACKET_BYTES, MISSING_DATA);
	}else if (*buffer != SESSION_START){
		return fatal_error_hangup(s, buffer, PACKET_BYTES, UNEXPECTED_FLAG);
	}
	//Check for matching version of FTP standard
	if ((*(buffer + offset_major_byte) != STANDARD_VERSION_MAJ) || (*(buffer + offset_minor_byte) != STANDARD_VERSION_MIN)){
		return fatal_error_hangup(s, buffer, PACKET_BYTES, VERSION_MISMATCH);
	}
	
	//Determine a session id for the client and send it
	struct queue *current_session = NULL;
	current_session = append_queue(qp, session_id_list, s);

	//Begin packet building
	memset(buffer,0,PACKET_BYTES);
	*buffer = SESSION_ID_FLAG;
	*(buffer + 1) = current_session->session_id;

	send(s, buffer, PACKET_BYTES, 0);

	//Receive a final confirmation
	if (read_buffer(s, buffer, PACKET_BYTES) != PACKET_BYTES)
		return fatal_error_hangup(s, buffer, PACKET_BYTES, MISSING_DATA);
	if (*buffer != SESSION_ID_FLAG || *(buffer + 1) != current_session->session_id)
		return fatal_error_hangup(s, buffer, PACKET_BYTES, CORRUPTED_FLAG);

	return FTP_TRUE;
}

//Takes incoming data, reverses byte order, determines what to do based on flag, returns proper flag
uint32_t incoming_data(int s, uint32_t *c, uint32_t *flag)
{
	/*memset(c,0,PACKET_BYTES);
	memset(flag,0,sizeof(uint32_t)); */
	if (read_buffer(s, c, PACKET_BYTES) == 0)
		return CORRUPTED_FLAG;

	decapsulate(c, flag);
	if (*flag == PAYLOAD)
		return PAYLOAD_BYTES;
	else if (*flag == META_FLAG)
		return PAYLOAD_BYTES;
	else if (*flag == DIFF_SIZE)
		//Data with a size_flag is the size in bytes of the next payload
		return *(c+1);
	else if (*flag == HASH_PAYLOAD)
		return HASH_PAYLOAD; //could be any large number
	else if (*flag == END_FLAG){
		//This flag should only be received by incoming_data_last
		return CORRUPTED_FLAG;
	}else if(*flag == END_SESSION){
		return END_SESSION;
	}
	return 0;
}

//Returns 0 on failure
uint32_t incoming_data_last(int s, uint32_t *c, uint32_t *flag, uint32_t expected_bytes)
{
	puts("Data last");
	memset(c,0,PACKET_BYTES);
	uint32_t r_tell = read_buffer(s, c, expected_bytes);

	decapsulate(c, flag);
	printf("Flag %x\n", *flag);
	printf("Length %d\n", expected_bytes);
	fwrite(c+1,expected_bytes,1,stdout);
	/*
	 * Not sure why this was here
	 * change when hash is reenabled
	if (*flag == PAYLOAD)
		return 1;
	
	else*/ if (*flag == END_FLAG)
		return 0;
	return 0;
}

//Compares hash from client to local hash, returns 1 on success
int compare_hash(uint32_t *hash_buff, uint32_t *c)
{
	//If too large, discard extra
	if (hash_buff[hashes] > REMOVE_FLAG)
		hash_buff[hashes] = hash_buff[hashes] & REMOVE_FLAG;
	//DEBUGGING
	fprintf(stdout, "Hashes: %x, %x\n", hash_buff[hashes], *c);
	if (hash_buff[hashes] == *c){
		hashes++;
		return 1;
	}else{
		hashes++;
		return 0;
	}
}

//Takes argv and an index to parse cli arguments, convert them to int, and set the appropriate values in the socket struct
int set_cli_parameter(char **arg, int i, struct ftp_sock *sock_params)
{
	int port = DEFAULT_PORT;
	int pending = DEFAULT_PENDING;
	if (strlen(arg[i]) > 2){
		return ARG_TOO_SMALL;
	}
	if (arg[i][0] != '-'){
		return POORLY_FORMED_ARGUMENT;
	}
	switch (arg[i][1]){
		case 'p':
			if ((port = atoi(arg[i+1])) == 0){
				return POORLY_FORMED_ARGUMENT;
			}
			sock_params->port = port;
			printf("Port set to %d...\n", sock_params->port);
			return SUCCESS;
		case 'c':
			if (pending = atoi(arg[i+1]) == 0){
				return POORLY_FORMED_ARGUMENT;
			}
			sock_params->max_pending_connections = pending;
			return SUCCESS;
		case 'h':
			printf("Allowed arguments:\n-p [Number]\n\tSpecify a port number.\n-c [Number]\n\tSpecify the number of maximum waiting connections on the socket.\n-h\n\tDisplay this message.\n");
			exit(0);
		default:
			return POORLY_FORMED_ARGUMENT;
	}
	return FAIL;
}

int main(int argc, char *argv[])
{
	struct sockaddr_in addrport, addrcli;
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int s; 
	
	struct ftp_sock *ftp_sock_parameters = (struct ftp_sock *)malloc(sizeof(struct ftp_sock));
	int invalid_command = FTP_FALSE;
	ftp_sock_parameters->port = DEFAULT_PORT;
	ftp_sock_parameters->max_pending_connections = DEFAULT_PENDING;

	if (argc > 1){
		for(int i = 1; i < argc; i+=2){
			invalid_command = set_cli_parameter(argv,i,ftp_sock_parameters);
			if(invalid_command){
				fprintf(stderr,"Invalid command line argument, pass -h for help");
				exit(COMMAND_ERROR);
			}
		}
	}else{
		//PLANNED to allow a user to enter values that would have been passed as cli arguments
		//input_socket_parameters(ftp_sock_parameters);
	}

	//Setup socket and ip address structs
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(ftp_sock_parameters->port);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//remove any garbage from addrcli memory space
	memset(&addrcli, 0, sizeof(addrcli));
	addrcli.sin_family = AF_INET;
	addrcli.sin_port = htons(INADDR_ANY);
	addrcli.sin_addr.s_addr = htonl(INADDR_ANY);

	puts("Socket created..");

	if (bind(sockid,(struct sockaddr *) &addrport,sizeof(addrport)) == FAIL_BIND_SOCKET){
		fprintf(stderr,"Could not bind socket");
		exit(FAIL_BIND_SOCKET);
	}
	puts("Socket bound..");
	
	FILE *fp;
	char *filen;

	struct queue *qp = queue_root();
	//be sure to free this later
	int *session_id_list = build_id_arr(MAX_ID);

	int status = listen(sockid, ftp_sock_parameters->max_pending_connections);
	for( ; ;){
		uint32_t *c = (uint32_t *)malloc(PACKET_BYTES);
		uint32_t *flag = (uint32_t *)malloc(sizeof(uint32_t));
		//Remove any stagnant garbage from these memory spaces
		*c = *c & 0;
		*flag = *flag & 0;
		uint32_t *hash_buff = (uint32_t *)malloc(sizeof(uint32_t) * 4);
		for (int i = 0; i < LENGTH_BUFFER; i++)
			hash_buff[i] = hash_buff[i] & 0;

		if (c == NULL)
			continue;

		socklen_t clilen = sizeof(addrcli);		
		s = accept(sockid, (struct sockaddr *) &addrcli, &clilen);
		puts("Connection established..");
		
		//Get init signal
		if(handshake_server(s, c, qp, session_id_list) != FTP_TRUE){
			fprintf(stdout, "Could not pass handshake with client.");
			//Write a retry loop
			continue;
		}

		//Take return of incoming_data() and process accordingly
		uint32_t size_message = 1;
		*flag = 0;

		uint32_t *c_data = c+1;
		memset(c,0,PACKET_BYTES);

		struct metadata *fp_meta = NULL;
		
		while (size_message = incoming_data(s, c, flag)){
			if (size_message == CORRUPTED_FLAG){
				printf("Received empty or corrupted packet. Ignoring..\n");
				continue;
			}
			if (*flag == END_SESSION){
				printf("Session ended by client.\n");

				//Do queue management here
				//return_id(session_id_list, 0);
				//remove_queue(root, session_id_list, s);

				if (fp_meta != NULL)
					free_metadata(fp_meta);
				break;
			}
			if (*flag == META_FLAG){
				fp_meta = pack_metadata_packet(c);
				//Open file
				fp = fopen(fp_meta->name, "wb");
				if(fp == NULL){
					fprintf(stderr, "Could not write file");
					exit(1);
				}
				printf("Writing file %s\t%d Bytes\n", fp_meta->name, fp_meta->size);
			}
			if (*flag == PAYLOAD){
				fwrite(c_data, PAYLOAD_BYTES, 1, fp);
				//hash_uint32(hash_buff, *c, hash_count++);
			}else if (size_message < PAYLOAD_BYTES){
				incoming_data_last(s, c, flag, size_message);
				fwrite(c_data, size_message - PAYLOAD_SIZE, 1, fp);
				
				//New file may be opened before connection closes
				if (fp_meta != NULL)
					free_metadata(fp_meta);
				//hash_uint32(hash_buff, *c, hash_count++);
				break;
			}
			/*
			 * Hashing disabled for testing
			if (size_message == HASH_PAYLOAD){
				if (!compare_hash(hash_buff, c)){
					fprintf(stdout, "File corruption detected. File hash mismatch. Please retry transfer. %x\n", hash_buff[hashes - 1]);
				}else{
					fprintf(stdout, "Matching file hash success: %x\n", hash_buff[hashes - 1]);
				}
			}
			*/
		}

		fclose(fp);
		puts("File write success");
		
		//Free any memory not in use until next connection and reset hash
		if (filen != NULL)
			free(filen);

		if (c != NULL)
			free(c);
		if (flag != NULL)
			free(flag);
		
		if (hash_buff != NULL)
			free(hash_buff);

		hash_count = 0;
		hashes = 0;
	}

	if (session_id_list != NULL)
		free(session_id_list);
	
	int statusc = close(sockid);
	close(s);
	return 0;
}
