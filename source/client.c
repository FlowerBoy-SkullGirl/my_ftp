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
	//used to 
	uint32_t *c_data = c+1;

	for (uint32_t i = ftell(fp); i < (endf - (PAYLOAD_BYTES)); i = ftell(fp)){
		fread(c_data, PAYLOAD_SIZE, (PAYLOAD_ARR_SIZE-1), fp);
		*c = encapsulate(PAYLOAD_FLAG, *c);
		send(sockid, c, PACKET_BYTES, 0);
	}

	*c = EMPTY_DATA;
	*c_data = EMPTY_DATA;
	*c = encapsulate(SIZE_FLAG, *c);
	*c_data = endf - ftell(fp);
	//This is to add the size of te flag which is a uint32_t type
	*c_data += PAYLOAD_SIZE;
	send(sockid, c, PACKET_BYTES, 0);
	memset(c,0,PACKET_BYTES);

	fread(c_data, 1, (endf - ftell(fp)), fp);
	*c = encapsulate(EOF_FLAG, *c);
	
	send(sockid, c, PACKET_BYTES, 0);
	memset(c,0,PACKET_BYTES);

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
		fp = fopen(filen, "rb");
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
	int handshake_error = handshake_client(sockid, sended);

	uint32_t *c = (uint32_t *)malloc(PAYLOAD_SIZE);
	if (c == NULL){
		puts("Could not allocate memory for filereader");
		exit(1);
	}
	//Sends contents of a file
	if (fp == NULL){
		fprintf(stderr, "File was closed. Ending transmission");
		//send an corrupt signal
		exit(0);
	}else if (handshake_error){
		fprintf(stderr, "Could not intialize connection.");
		exit(0);
	}

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
	if(!gotit){
		fprintf(stderr, "Did not receive response. Ending session.");
		exit(0);
	}
	puts("Server received filen");
	
	c = (uint32_t *)realloc(c,PACKET_BYTES);
	memset(c,0,(PACKET_BYTES));

	int flag = send_arr(sockid, fp, c);
	
	if (flag){
		fprintf(stdout,"Error: data not sent. %d bits.",flag);
	}

	fclose(fp);

	/* 
	 * Temporarily disable hashing
	 *
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
	*/

	fprintf(stdout, "\n"); //create newline
	//Inform server that all data is received
	*c & EMPTY_DATA;
	*c = END_FLAG;
	send(sockid, c, PACKET_BYTES, 0);

	if (c != NULL)
		free(c);

	puts("Sent end of file flag");
	//close connection
	int statusc = close(sockid);

	return 0;
}
