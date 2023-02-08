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

#define FAIL_BIND_SOCKET (-1)
#define COMMAND_ERROR 1
#define ARG_TOO_SMALL 1
#define POORLY_FORMED_ARGUMENT 2
#define SUCCESS 0
#define FAIL 1

//Global variable used to store file hash and verify integrity	
uint32_t hash_count = 0;
int hashes = 0;

//Returns 0 on success, sends confirmation to client before more data is received
int getfilen(int s, char **filen){
	int gotname = 1;
	uint32_t size_filen = 0;
	char gotsize = 1;
	//Receive size of filen
	recv(s, &size_filen, sizeof(uint32_t), 0);
	send(s, &gotsize, 1, 0);
	size_filen = ntohl(size_filen);

	char *filentemp = (char *)malloc(size_filen);
	//Receive a filename from the client	
	recv(s, filentemp, MAXLEN, 0);

	int path = 1;
	while(path){
		path = strcspn(filentemp, "/");
		if (path != strlen(filentemp)){
			memcpy(filentemp, &filentemp[path+1], (strlen(filentemp) - path));
			path = 1;
		}else{
			path = 0;
		}
	}
	*filen = filentemp;
	printf("%s %d\n", filentemp, size_filen);
	filentemp[size_filen - 1] = '\0';
	printf("Received filename from client: %s\nSize: %ld\n", *filen, size_filen);
	
	//Confirm filename was received
	gotname = htonl(gotname);
	send(s, &gotname, sizeof(gotname), 0);
		
	return 0;
}
	
//Takes incoming data, reverses byte order, determines what to do based on flag, returns proper flag
uint32_t incoming_data(int s, uint32_t *c, uint32_t *flag)
{
	recv(s, c, sizeof(uint32_t)*PAYLOAD_ARR_SIZE, 0);
	decapsulate(c, flag);
	if (*flag == PAYLOAD)
		return PAYLOAD_ARR_SIZE*PAYLOAD_SIZE;
	else if (*flag == DIFF_SIZE)
		//Data with a size_flag is the size in bytes of the next payload
		return *(c+1);
	else if (*flag == HASH_PAYLOAD)
		return HASH_PAYLOAD; //could be any large number
	else if (*flag == EOF_FLAG)
		return 0;
	return 0;
}

//Returns 0 on failure
uint32_t incoming_data_last(int s, uint32_t *c, uint32_t *flag)
{
	recv(s, c, sizeof(uint32_t)*PAYLOAD_ARR_SIZE, 0);
	decapsulate(c, flag);
	/*
	 * Not sure why this was here
	 * change when hash is reenabled
	if (*flag == PAYLOAD)
		return 1;
	
	else*/ if (*flag == EOF_FLAG)
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

	int status = listen(sockid, ftp_sock_parameters->max_pending_connections);
	for( ; ;){
		uint32_t *c = (uint32_t *)malloc(sizeof(uint32_t));
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
		if(handshake_server(s) != PASS_HANDSHAKE){
			fprintf(stdout, "Could not pass handshake with client.");
			//Write a retry loop
			continue;
		}
		//Receive filen
		getfilen(s, &filen);
		printf("Writing file %s\n", filen);

		//Open file
		fp = fopen(filen, "w");
		if(fp == NULL){
			fprintf(stderr, "Could not write file");
			exit(1);
		}

		//Take return of incoming_data() and process accordingly
		uint32_t size_message = 1;
		*flag = 0;

		c = (uint32_t *)realloc(c,sizeof(uint32_t)*(PAYLOAD_ARR_SIZE));
		memset(c,0,(PAYLOAD_ARR_SIZE*PAYLOAD_SIZE));

		while (size_message = incoming_data(s, c, flag)){
			if (*flag == PAYLOAD){
				fwrite(c+1, size_message, 1, fp);
				//hash_uint32(hash_buff, *c, hash_count++);
			}else if (size_message < PAYLOAD_SIZE*PAYLOAD_ARR_SIZE){
				incoming_data_last(s, c, flag);
				fwrite(c+1, size_message, 1, fp);
				//hash_uint32(hash_buff, *c, hash_count++);
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
	
	
	int statusc = close(sockid);
	close(s);
	return 0;
}
