#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include "hashr.h" //Hash algorithm for validating file integrity
#include "networking.h" //Defines for flags. Functions for send and receive

int send_arr(int sockid, FILE *fp, uint32_t *c)
{
	fseek(fp, 0, SEEK_END);
	long endf = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("Endf %d\n", endf);

	for (uint32_t i = ftell(fp); i < (endf - (PAYLOAD_ARR_SIZE*PAYLOAD_SIZE)); i = ftell(fp)){
		puts("Inner");
		fread((c+1), PAYLOAD_SIZE, (PAYLOAD_ARR_SIZE-1), fp);
		*(c) = encapsulate(PAYLOAD_FLAG, *(c));
		printf("%c\n",*c);
		fwrite(c,sizeof(uint32_t),PAYLOAD_ARR_SIZE,stdout);
		send(sockid, c, (sizeof(uint32_t)*PAYLOAD_ARR_SIZE), 0);
		memset(c,0,PAYLOAD_ARR_SIZE*sizeof(uint32_t));
	}

	
	uint32_t payload_remaining = endf - ftell(fp);
	printf("Payload size: %d\n", payload_remaining);
	payload_remaining = encapsulate(SIZE_FLAG, payload_remaining);
	*c = payload_remaining;
	int result = send(sockid, c, sizeof(uint32_t)*PAYLOAD_ARR_SIZE, 0);
	printf("Sent %x\n", *c);
	printf("Result %x\n", result);

	memset(c,0,sizeof(uint32_t)*PAYLOAD_ARR_SIZE);
	fread((c+1), PAYLOAD_SIZE, (endf - ftell(fp)), fp);
	*(c) = encapsulate(PAYLOAD_FLAG, *(c));
	fwrite(c,sizeof(uint32_t),PAYLOAD_ARR_SIZE,stdout);
	printf("Payload size in bytes %x\n",*c);
	printf("First payload uint: %x\n", *(c+1));

	result = send(sockid, c, (sizeof(uint32_t)*PAYLOAD_ARR_SIZE), 0);
	printf("First bytes: %x\n", *c);
	memset(c,0,PAYLOAD_ARR_SIZE*sizeof(uint32_t));

	printf("Result %x\n", result);
	perror("Err");

	recv(sockid,c,sizeof(uint32_t)*PAYLOAD_ARR_SIZE,0);
	puts("received");

	return endf - ftell(fp);
}

int main()
{
	//Client socket
	struct sockaddr_in addrserv;
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
	int status = -1;

	memset(&addrserv, 0, sizeof(addrserv));		
	addrserv.sin_family = AF_INET;
	addrserv.sin_port = htons(4414);
	addrserv.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	puts("Client started");
	while (status){
		status = connect(sockid, (struct sockaddr*) &addrserv, sizeof(addrserv));
		printf("Connected to server %d\n", status);
	}

	FILE *fp = fopen("client.test","r");
	uint32_t *c = (uint32_t *)malloc(sizeof(uint32_t)*PAYLOAD_ARR_SIZE);

	send_arr(sockid, fp, c);
	return 0;
}


