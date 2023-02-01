#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include "hashr.h" 
#include "neworking.h" //Most defines for flags are in this header

#define WAITERS 1
#define REMOVE_FLAG 0x0FFFFFFF
#define PAYLOAD_SIZE 4
#define PAYLOAD_ARR_SIZE 128
#define PASS_HANDSHAKE 0

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
	
//Separates data and flag, returns data
uint32_t *decapsulate(uint32_t *data, uint32_t *flag)
{
	*flag = 0xF0000000 & *data;
	*data = *data & REMOVE_FLAG;
	return data;
}

//Return 0 on successful handshake
int handshake_server(int s){
	uint32_t message = 0;
	uint32_t *mp = &message;
	uint32_t *flag = (uint32_t *)malloc(sizeof(uint32_t));
	//Gets message from client
	recv(s, &message, sizeof(uint32_t), 0);
	printf("Client sent init message\n");

	//Reorganize received data into host order
	message = ntohl(message);	
	decapsulate(mp, flag);	
	//If message is correct, send an int to confirm server is ready for more data
	if(*flag != INIT_FLAG){
		fprintf(stderr, "Did not receive correct signal from client");
		if (flag != NULL)
			free(flag);
		return 1;
	}
	puts("Expected init received...replying");
	message = htonl(INIT_FLAG);
	send(s, &message, sizeof(uint32_t), 0);

	if (flag != NULL)
		free(flag);
	return PASS_HANDSHAKE;
}
	
//Takes incoming data, reverses byte order, determines what to do based on flag, returns proper flag
uint32_t incoming_data(int s, uint32_t *c, uint32_t *flag)
{
	recv(s, c, sizeof(uint32_t)*PAYLOAD_ARR_SIZE, 0);
	decapsulate((c+127), flag);
	if (*flag == PAYLOAD)
		return PAYLOAD_ARR_SIZE;
	else if (*flag == DIFF_SIZE)
		//Data with a size_flag is the size in bytes of the next payload
		return *c;
	else if (*flag == HASH_PAYLOAD)
		return HASH_PAYLOAD; //could be any large number
	else if (*flag == EOF_FLAG)
		return 0;
	return 0;
}

//Returns 0 on failure
uint32_t incoming_data_last(int s, uint32_t *c, uint32_t *flag)
{
	recv(s, c, sizeof(uint32_t), 0);
	*c = ntohl(*c);
	c = decapsulate(c, flag);
	if (*flag == PAYLOAD)
		return 1;
	else if (*flag == EOF_FLAG)
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

int main()
{
	struct sockaddr_in addrport, addrcli;
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int s; 
	
	puts("Socket created..");

	//Setup socket and ip address structs
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(4414);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	
	memset(&addrcli, 0, sizeof(addrcli));
	addrcli.sin_family = AF_INET;
	addrcli.sin_port = htons(INADDR_ANY);
	addrcli.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockid, (struct sockaddr *) &addrport, sizeof(addrport)) == -1){
		fprintf(stderr,"Could not bind socket");
	}
	puts("Socket bound..");
	
	FILE *fp;
	char *filen;

	int status = listen(sockid, WAITERS);
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
		memset(c,0,(PAYLOAD_ARR_SIZE));

		while (size_message = incoming_data(s, c, flag)){
			if (*flag == PAYLOAD){
				fwrite(c, size_message, PAYLOAD_ARR_SIZE, fp);
				hash_uint32(hash_buff, *c, hash_count++);
			}else if (size_message < PAYLOAD_SIZE){
				incoming_data_last(s, c, flag);
				fwrite(c, size_message, 1, fp);
				hash_uint32(hash_buff, *c, hash_count++);
			}
			if (size_message == HASH_PAYLOAD){
				if (!compare_hash(hash_buff, c)){
					fprintf(stdout, "File corruption detected. File hash mismatch. Please retry transfer. %x\n", hash_buff[hashes - 1]);
				}else{
					fprintf(stdout, "Matching file hash success: %x\n", hash_buff[hashes - 1]);
				}
			}
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
