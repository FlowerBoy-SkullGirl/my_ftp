//Arguments command: server-ip file-to-send
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include "hashr.h" //Hash algorithm for validating file integrity
#include "networking.h" //Defines for flags. Functions for send and receive
#include "session_queue.h"
#include "file_metadata.h"

#define MAXLEN_FILE_LIST 256
#define MAX_RETRY_CONNECT 20

//Struct of inet_addr for reference
/*	unsigned int inet_addr(char *str){
		int a, b, c, d;
		char arr[4];
		sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
		arr[0] = a; arr[1] = b, arr[2] = c, arr[3] = d;
		return *(unsigned int *)arr;
	}
*/	

int send_metadata(int sockid, struct metadata file_meta, uint32_t *buffer, uint32_t session_mask)
{
	if (!build_metadata_packet(file_meta, buffer, session_mask)){
		fprintf(stderr, "Could not arrange metadata\n");
		exit(FTP_FALSE);
	}
	send(sockid, buffer, PACKET_BYTES, 0);

	return FTP_TRUE;
}

int handshake_client(int s, uint32_t *buffer, int *session_id)
{
	int offset_major_byte = 1;
	int offset_minor_byte = 2;

	memset(buffer, 0, PACKET_BYTES);
	*buffer = SESSION_START;
	*(buffer + offset_major_byte) = STANDARD_VERSION_MAJ;
	*(buffer + offset_minor_byte) = STANDARD_VERSION_MIN;
	
	send(s, buffer, PACKET_BYTES, 0);

	if (read_buffer(s, buffer, PACKET_BYTES) != PACKET_BYTES)
		return fatal_error_hangup(s, buffer, PACKET_BYTES, MISSING_DATA);
	if (*buffer == END_SESSION){
		fprintf(stderr, "Server returned error %x\n", *(buffer+1));
		exit(FTP_FALSE);
	}
	if (*buffer != SESSION_ID_FLAG)
		return fatal_error_hangup(s, buffer, PACKET_BYTES, UNEXPECTED_FLAG);

	*session_id = *(buffer + 1);
	
	//Confirm receipt of correct id
	send(s, buffer, PACKET_BYTES, 0);

	return FTP_TRUE;
}

int send_arr(int sockid, FILE *fp, uint32_t *c)
{
	fseek(fp, 0, SEEK_END);
	long endf = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	//used to avoid (c+1) being reused everywhere
	uint32_t *c_data = c+1;

	printf("Sending main payload\n");
	for (uint32_t i = ftell(fp); i < (endf - (PAYLOAD_BYTES)); i = ftell(fp)){
		fread(c_data, PAYLOAD_SIZE, (PAYLOAD_ARR_SIZE-1), fp);
		*c = PAYLOAD;
		send(sockid, c, PACKET_BYTES, 0);
	}

	*c = EMPTY_DATA;
	*c_data = EMPTY_DATA;
	*c = DIFF_SIZE;
	*c_data = endf - ftell(fp);
	printf("Sending remainder size %d\n", *c_data);

	send(sockid, c, PACKET_BYTES, 0);
	memset(c,0,PACKET_BYTES);

	fread(c_data, 1, (endf - ftell(fp)), fp);
	*c = EOF_FLAG;
	
	printf("Sending EOF\n");
	send(sockid, c, PACKET_BYTES, 0);

	return endf - ftell(fp);
}

int main(int argc, char *argv[]){
	struct sockaddr_in addrserv;
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	char server_ip[MAXLEN];
	char recieveded[MAXLEN];
	char filen_list[MAXLEN_FILE_LIST][MAXLEN];
	char filen[MAXLEN];
	char buff[MAXLEN];
	int num_of_files = 0;

	//Get ip, assign to str
	if(argc < 2){
		puts("Enter the ipv4 address of the server:");
		scanf("%s", server_ip);
	}else{
		strcpy(server_ip, argv[1]);
	}
	
	//Get file to transfer, assign filen
	if(argc == 3){
		strcpy(filen_list[0], argv[2]);
		num_of_files++;
	}else if(argc < 3){
		puts("Enter the name of the file to be sent(with full path):");
		scanf("%s", filen_list[0]);
		num_of_files++;
	}
	else if(argc > 3){
		if (argc > MAXLEN_FILE_LIST){
			printf("File count is over limit of %d\n", MAXLEN_FILE_LIST);
			exit(FTP_FALSE);
		}
		for (int i = 2; i < argc; i++){
			strcpy(filen_list[i-2], argv[i]);
			num_of_files++;
		}
	}

	FILE **fp = (FILE **)malloc(sizeof(FILE *) * num_of_files);

	for (int i = 0; i < num_of_files; i++){
		fp[i] = fopen(filen_list[i], "rb");
		if(fp[i] == NULL){
			fprintf(stderr, "Could not open file of index: %d\n", i);
			exit(FTP_FALSE);
		}
	}

	//Sets the server address and port
	memset(&addrserv, 0, sizeof(addrserv));		
	addrserv.sin_family = AF_INET;
	addrserv.sin_port = htons(4414);
	addrserv.sin_addr.s_addr = inet_addr(server_ip);
	
	printf("Connecting to %s\n", server_ip);

	//Try connect to server. Returns 0 on success
	//To enforce max number of retries, add && num_retries <= MAX_RETRY_CONNECT
	int status = 1;
	while (status){
		status = connect(sockid, (struct sockaddr *) &addrserv, sizeof(addrserv));
		if (status){
			printf("Failed to connect. Retrying every 5 seconds.\n");
			sleep(5);
		}
	}

	//Init and end session codes
	char sended[MAXLEN] = "Hello, server\n";
	char endsended[MAXLEN] = "Goodbye, server\n";
	int gotit = 0;	
	int length_sent = 0;
	char success = 1;
	char failure = 0;
	char recieved = 0;

	uint32_t *c = (uint32_t *)malloc(PACKET_BYTES);
	int *session_id = (int *) malloc(sizeof(int));

	//Send preliminary hello
	int handshake_error = handshake_client(sockid, c, session_id);

	uint32_t session_mask = *session_id;
	free(session_id);

	if (c == NULL){
		puts("Could not allocate memory for buffer");
		exit(1);
	}

	//Sends contents of a file
	if (handshake_error == FTP_FALSE){
		fprintf(stderr, "Could not intialize connection.");
		exit(0);
	}
	puts("Handshake to server success.");

	struct metadata *fp_meta[MAXLEN_FILE_LIST];
	//Truncate file paths and build metadata structs
	for (int i = 0; i < num_of_files; i++){
		char *filen_nopath = truncate_file_path(filen_list[i]);
		//Pack_File allocates memory, must free later with free_metadata
		fp_meta[i] = pack_file_data(fp[i], filen_nopath);
		printf("Preparing metadata of file %s of size %d bytes\n", fp_meta[i]->name, fp_meta[i]->size);

		if (filen_nopath != NULL)
			free(filen_nopath);
	}

	//main loop

	int files_sent = 0;
	while (num_of_files > 0){
		send_metadata(sockid, *fp_meta[files_sent], c, session_mask); 
		printf("Sent metadata to server for %s\n", fp_meta[files_sent]->name);

		int flag = send_arr(sockid, fp[files_sent], c);
	
		if (flag){
			fprintf(stdout,"Error: data not sent. %d bits.",flag);
		}

		//Send server the hash of the file
		fprintf(stdout, "Sending hash to server.\n");

		//Hash_file malloc's memory for the string
		char *file_hash = hash_file(fp[files_sent]);
		memset(c, 0, PACKET_BYTES);

		build_hash_packet(file_hash, c, session_mask); 
		send(sockid, c, PACKET_BYTES, 0);

		//Free the hash string that is no longer needed
		if (file_hash != NULL){
			free(file_hash);
			file_hash = NULL;
		}

		fclose(fp[files_sent]);
		printf("File sent successfully\n");

		--num_of_files;
		files_sent++;
		//Indicate that an additional file will be sent
		if (num_of_files > 0){
			*c = NEXT_FLAG;
			send(sockid, c, PACKET_BYTES, 0);
		}
	}

	//Inform server that all data is received
	*c & EMPTY_DATA;
	*c = END_SESSION;
	send(sockid, c, PACKET_BYTES, 0);
	printf("Ending session with server.\n");

	if (c != NULL)
		free(c);
	for (int i = 0; i < files_sent; i++){
		if (fp_meta[i] != NULL)
			free_metadata(fp_meta[i]);
	}
	//close connection
	int statusc = close(sockid);

	return 0;
}
