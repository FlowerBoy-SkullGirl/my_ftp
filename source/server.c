#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
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

int handshake_server(int s, uint32_t *buffer, struct queue *qp, int *session_id_list, FILE *print_fd)
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

	fprintf(print_fd, "Assigned session id to client\n");

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

//Takes argv and an index to parse cli arguments, convert them to int, and set the appropriate values in the socket struct
int set_cli_parameter(char **arg, int i, struct ftp_sock *sock_params, FILE **print_fd)
{
	int port = DEFAULT_PORT;
	int pending = DEFAULT_PENDING;
	char *logfilen = "serverlog";
	if (strlen(arg[i]) < 2){
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
			return SUCCESS;
		case 'c':
			if (pending = atoi(arg[i+1]) == 0){
				return POORLY_FORMED_ARGUMENT;
			}
			sock_params->max_pending_connections = pending;
			return SUCCESS;
		case 'h':
			fprintf(stdout, "Allowed arguments:\n-p [Number]\n\tSpecify a port number.\n-c [Number]\n\tSpecify the number of maximum waiting connections on the socket.\n-h\n\tDisplay this message.\n");
			exit(0);
		case 'q':
			*print_fd = fopen(logfilen, "a");
			if (print_fd == NULL){
				fprintf(stderr, "Could not open log file\n");
				exit(0);
			}
			fprintf(*print_fd, "Outputting to log file: %s\n", logfilen);
			return SUCCESS;
			break;
		default:
			return POORLY_FORMED_ARGUMENT;
	}
	return FAIL;
}

char *prepend_root(char *filen, char *path)
{
	char *filen_w_path = (char *) malloc(strlen(filen) + strlen(path) + 1);
	int i = 0;
	for (; i < strlen(path); i++){
		*(filen_w_path + i) = *(path + i);
	}
	for (int j = 0; j < strlen(filen) + 1; j++){
		*(filen_w_path + i + j) = *(filen + j);
		//Overwrite prepended '/' on filename
		if (j == 0 && filen[0] == '/')
			i--;
	}

	free(filen);

	return filen_w_path;
}

int make_paths(char *filen)
{
	char temp[MAXLEN];
	int NOT_DIR = -1;
	struct stat status = { 0 };
	int dir_success = 0;
	for (int i = 0; i < strlen(filen); i++)
	{
		if (filen[i] == '/'){
			snprintf(temp, i + 1, "%s", filen);
			if (stat(temp, &status) == NOT_DIR)
				dir_success = mkdir(temp, 0700);
			if (dir_success == NOT_DIR){
				fprintf(stderr, "Could not open directory\n");
				exit(FTP_FALSE);
			}
		}
	}
	return FTP_TRUE;
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

	FILE *print_fd = NULL;

	if (argc > 1){
		for(int i = 1; i < argc; i++){
			//If i'th arg is a value and not a flag
			if (argv[i][0] != '-')
				continue;
			invalid_command = set_cli_parameter(argv,i,ftp_sock_parameters, &print_fd);
			if(invalid_command){
				fprintf(stderr,"Invalid command line argument, pass -h for help");
				exit(COMMAND_ERROR);
			}
		}
	}else{
		//PLANNED to allow a user to enter values that would have been passed as cli arguments
		//input_socket_parameters(ftp_sock_parameters);
	}

	//If print_fd was not set to quiet in cli arguments
	if (print_fd == NULL){
		puts("Printing to stdout...");
		print_fd = stdout;
	}

	fprintf(print_fd, "Port set to %d...\n", ftp_sock_parameters->port);

	//Setup socket and ip address structs
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(ftp_sock_parameters->port);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//remove any garbage from addrcli memory space
	memset(&addrcli, 0, sizeof(addrcli));
	addrcli.sin_family = AF_INET;
	addrcli.sin_port = htons(INADDR_ANY);
	addrcli.sin_addr.s_addr = htonl(INADDR_ANY);

	fprintf(print_fd, "Socket created..\n");

	if (bind(sockid,(struct sockaddr *) &addrport,sizeof(addrport)) == FAIL_BIND_SOCKET){
		fprintf(stderr,"Could not bind socket");
		exit(FAIL_BIND_SOCKET);
	}
	fprintf(print_fd, "Socket bound..\n");
	
	FILE *fp;
	//path where all files are to be written
	char *f_path = "./";

	struct queue *qp = queue_root();
	//be sure to free this later
	int *session_id_list = build_id_arr(MAX_ID);

	int status = listen(sockid, ftp_sock_parameters->max_pending_connections);
	for( ; ;){
		uint32_t *c = (uint32_t *)malloc(PACKET_BYTES);
		//Remove any stagnant garbage from these memory spaces
		*c = *c & 0;
		if (c == NULL)
			continue;

		socklen_t clilen = sizeof(addrcli);		
		s = accept(sockid, (struct sockaddr *) &addrcli, &clilen);
		fprintf(print_fd, "Connection established..\n");
		
		//Get init signal
		if(handshake_server(s, c, qp, session_id_list, print_fd) != FTP_TRUE){
			fprintf(print_fd, "Could not pass handshake with client.\n");
			//Write a retry loop
			continue;
		}
		fprintf(print_fd, "Passed handshake with client.\n");

		//Take return of incoming_data() and process accordingly
		uint32_t size_message = 1;

		uint32_t *c_data = c+1;
		memset(c,0,PACKET_BYTES);

		struct metadata *fp_meta = NULL;
		
		/*
		 * Main loop for each connection.
		 * First 4 bytes will be a flag
		 * Remaining bytes are data
		 */
		uint32_t expected_bytes = PACKET_BYTES;
		while (size_message = read_buffer(s, c, PACKET_BYTES)){
			if (size_message == CORRUPTED_FLAG){
				fprintf(print_fd, "Received empty or corrupted packet. Ignoring..\n");
				continue;
			}
			if (*c == END_SESSION){
				fprintf(print_fd, "Session ended by client.\n");

				//Do queue management here
				//return_id(session_id_list, 0);
				//remove_queue(root, session_id_list, s);

				if (fp_meta != NULL){
					free_metadata(fp_meta);
					fp_meta = NULL;
				}
				break;
			}
			else if (*c == META_FLAG){
				fp_meta = pack_metadata_packet(c);
				fp_meta->name = prepend_root(fp_meta->name, f_path);
				make_paths(fp_meta->name);
				//Open file
				fp = fopen(fp_meta->name, "wb");
				if(fp == NULL){
					fprintf(stderr, "Could not write file");
					exit(1);
				}
				fprintf(print_fd, "Writing file %s\t%ld Bytes\n", fp_meta->name, fp_meta->size);
			}
			else if (*c == PAYLOAD){
				fwrite(c_data, PAYLOAD_BYTES, 1, fp);
			}
			else if (*c == DIFF_SIZE)
			{
				expected_bytes = *(c + 1);
				fprintf(print_fd, "Last payload is %d\n", expected_bytes);
			}
			else if (*c == EOF_FLAG){
				fwrite(c_data, expected_bytes, 1, fp);
				
				fprintf(print_fd, "Client sent EOF. File write success\n");
			}else if (*c == HASH_PAYLOAD){
				//Open the file for reading
				if (fp != NULL){
					fclose(fp);
					fp = NULL;
				}
				fp = fopen(fp_meta->name, "r");

				//unpack_hash malloc's memory for client_hash
				char *client_hash = unpack_hash_packet(c);
				char *local_hash = hash_file(fp);
				fprintf(print_fd, "%s\tClient copy\n", client_hash);
				fprintf(print_fd, "%s\tLocal copy\n", local_hash);
				//Compare hashes
				if (strcmp(client_hash, local_hash) == 0){
					fprintf(print_fd, "Hashes match successfully\n");
				}else{
					fprintf(print_fd, "Hashes do not match! File corrupted\n");
				}
				//Free strings
				if (client_hash != NULL){
					free(client_hash);
					client_hash = NULL;
				}
				if (local_hash != NULL){
					free(local_hash);
					local_hash = NULL;
				}
			}else if (*c == NEXT_FLAG){
				//New file may be opened before connection closes
				if (fp_meta != NULL){
					free_metadata(fp_meta);
					fp_meta = NULL;
				}
				if (fp != NULL){
					fclose(fp);
					fp = NULL;
				}
				fprintf(print_fd, "Client is preparing next file...\n");
			}else{
				fprintf(print_fd, "Received empty or corrupted packet. Ignoring..\n");
				continue;
			}
		}


		if (fp != NULL){
			fclose(fp);
			fp = NULL;
		}
		
		if (c != NULL){
			free(c);
			c = NULL;
		}
		
		if (fp_meta != NULL){
			free_metadata(fp_meta);
			fp_meta = NULL;
		}

		//make sure log is written to disk
		fflush(print_fd);
	}

	if (session_id_list != NULL){
		free(session_id_list);
		session_id_list = NULL;
	}

	if (print_fd != stdout && print_fd != NULL){
		fclose(print_fd);
		print_fd = NULL;
	}
	
	int statusc = close(sockid);
	close(s);
	return 0;
}
