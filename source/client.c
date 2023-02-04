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

//Global hash value to verify data integrity after transmission
uint32_t hash_total[LENGTH_BUFFER]; //Number of uint32_t in hash
uint32_t hash_count = 0;
	
//Struct of inet_addr for reference
/*	unsigned int inet_addr(char *str){
		int a, b, c, d;
		char arr[4];
		sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
		arr[0] = a; arr[1] = b, arr[2] = c, arr[3] = d;
		return *(unsigned int *)arr;
	}
*/	

int send_arr(int sockid, FILE *fp, uint32_t *c)
{
	fseek(fp, 0, SEEK_END);
	long endf = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	for (uint32_t i = ftell(fp); i < (endf - (PAYLOAD_ARR_SIZE*PAYLOAD_SIZE)); i = ftell(fp)){
		puts("Inner");
		fread(c, PAYLOAD_SIZE, (PAYLOAD_ARR_SIZE-1), fp);
		*(c+PAYLOAD_ARR_SIZE-1) = encapsulate(PAYLOAD_FLAG, *(c+PAYLOAD_ARR_SIZE-1));
		printf("%c\n",*c);
		fwrite(c,sizeof(uint32_t),PAYLOAD_ARR_SIZE,stdout);
		send(sockid, c, (sizeof(uint32_t)*PAYLOAD_ARR_SIZE), 0);
		memset(c,0,PAYLOAD_ARR_SIZE);
	}

	fread(c, PAYLOAD_SIZE, (endf - ftell(fp)), fp);
	*(c+PAYLOAD_ARR_SIZE-1) = encapsulate(PAYLOAD_FLAG, *(c+PAYLOAD_ARR_SIZE-1));
	printf("%d\n",*(c+PAYLOAD_ARR_SIZE-1));
	fwrite(c,sizeof(uint32_t),PAYLOAD_ARR_SIZE,stdout);
	
	uint32_t payload_remaining = endf - PAYLOAD_ARR_SIZE;
	payload_remaining = encapsulate(SIZE_FLAG, payload_remaining);
	payload_remaining = htonl(payload_remaining);
	send(sockid, &payload_remaining, sizeof(uint32_t), 0);

	send(sockid, c, (sizeof(uint32_t)*PAYLOAD_ARR_SIZE), 0);
	memset(c,0,PAYLOAD_ARR_SIZE);

	return endf - ftell(fp);
}

//Return value is size of remaining file
int send_payload(int sockid, FILE *fp, uint32_t *c)
{
	fseek(fp, 0, SEEK_END);
	uint32_t endf = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	for (uint32_t i = ftell(fp); i <= (endf - PAYLOAD_SIZE); i = ftell(fp)){
		fread(c, PAYLOAD_ARR_SIZE, 1, fp);
		hash_uint32(hash_total, *c, hash_count++);
		*c = encapsulate(PAYLOAD_FLAG, *c);
		*c = htonl(*c);
		send(sockid, c, sizeof(uint32_t), 0);
		*c = *c & EMPTY_DATA;
	}
	return endf - ftell(fp);
}

int main(int argc, char *argv[]){
	struct sockaddr_in addrserv;
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	char str[MAXLEN];
	char recieveded[MAXLEN];
	char filen[MAXLEN];
	char buff[MAXLEN];

	//Get ip, assign to str
	if(argc < 1){
		puts("enter ip addr");
		scanf("%s", str);
	}else{
		strcpy(str, argv[1]);
	}
	
	FILE *fp;
	//Get file to transfer, assign filen
	if(argc == 3){
		strcpy(filen, argv[2]);
		fp = fopen(filen, "r");
		if(fp == NULL){
			fprintf(stderr, "Could not open file\n");
			exit(1);
		}
	}else if(argc > 1 && argc < 3){
		puts("enter filename");
		scanf("%s", filen);
	}

	//Sets the server address and port
	memset(&addrserv, 0, sizeof(addrserv));		
	addrserv.sin_family = AF_INET;
	addrserv.sin_port = htons(4414);
	addrserv.sin_addr.s_addr = inet_addr(str);
	
	printf("Connecting to %s\n", str);

	//Try connect to server
	int status = connect(sockid, (struct sockaddr *) &addrserv, sizeof(addrserv));
	

	//Init and end session codes
	char sended[MAXLEN] = "Hello, server\n";
	char endsended[MAXLEN] = "Goodbye, server\n";
	int gotit = 0;	
	int length_sent = 0;
	char success = 1;
	char failure = 0;
	char recieved = 0;

	//Send preliminary hello
	int serverready = handshake_client(sockid, sended);

	uint32_t *c = (uint32_t *)malloc(32);
	if (c == NULL){
		puts("Could not allocate memory for filereader");
		exit(1);
	}
	//Sends contents of a file
	if(fp != NULL && !serverready){
		//truncate filename before transmission
		char *filenout = (char *)malloc(strlen(filen)+1);
		strcpy(filenout, filen);
		int path = 1;
		while(path){
			path = strcspn(filenout, "/");
			if (path != strlen(filenout)){
				memcpy(filenout, &filenout[path+1], (strlen(filenout) - path));
				path = 1;
			}else{
				path = 0;
			}
		}
		filenout[strlen(filenout)+1] = '\0';

		//send size filename
		uint32_t filensize = strlen(filenout) + 1;
		char gotfilesize = 0;
		filensize = htonl(filensize);
		send(sockid, &filensize, sizeof(uint32_t), 0);
		recv(sockid, &gotfilesize, 1, 0);

		//Send filen
		send(sockid, filenout, strlen(filenout), 0);
		printf("Sent filename: %s\n", filenout);
		recv(sockid, &gotit, MAXLEN, 0);
		//Make sure bytes arrived in order
		gotit = ntohl(gotit);	
		
		printf("Server returned: %d\n", gotit);

		//main loop
		if(gotit){
			puts("Server received filen");
			
			uint32_t *c = (uint32_t *)realloc(c,32*(PAYLOAD_ARR_SIZE));
			memset(c,0,(PAYLOAD_ARR_SIZE));

			int flag = send_arr(sockid, fp, c);
			
			if (flag){
				uint32_t payload_remaining = flag;
				payload_remaining = encapsulate(SIZE_FLAG, payload_remaining);
				payload_remaining = htonl(payload_remaining);
				send(sockid, &payload_remaining, sizeof(uint32_t), 0);

				fread(c, flag, 1, fp);
				hash_uint32(hash_total, *c, hash_count++);
				*c = encapsulate(PAYLOAD_FLAG, *c);
				*c = htonl(*c);
				send(sockid, c, sizeof(uint32_t), 0);
			}
		}			 
		fclose(fp);
	}
	if (c != NULL)
		free(c);
	//Send server the hash of the file
	fprintf(stdout, "Sending hash to server: ");
	for (int i = 0; i < LENGTH_BUFFER; i++){
		if (hash_total[i] > REMOVE_FLAG)
			hash_total[i] = hash_total[i] & REMOVE_FLAG;
		hash_total[i] = encapsulate(HASH_FLAG, hash_total[i]);
		hash_total[i] = htonl(hash_total[i]);
		send(sockid, hash_total + i, sizeof(uint32_t), 0);
		fprintf(stdout, "%x", hash_total[i]);
	}
	fprintf(stdout, "\n"); //create newline
	//Inform server that all data is received
	uint32_t end_signal_client = END_FLAG;
	end_signal_client = htonl(end_signal_client);
	send(sockid, &end_signal_client, 1, 0);
	puts("Sent end of file flag");
	//close connection
	int statusc = close(sockid);

	return 0;
}
