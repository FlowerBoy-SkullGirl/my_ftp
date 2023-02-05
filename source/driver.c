#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "hashr.h" //Hash algorithm for validating file integrity
#include "networking.h" //Defines for flags. Functions for send and receive

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

void *launch_client(void *arg)
{
	//Client socket
	struct sockaddr_in addrserv;
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 

	memset(&addrserv, 0, sizeof(addrserv));		
	addrserv.sin_family = AF_INET;
	addrserv.sin_port = htons(4414);
	addrserv.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	puts("Client started");
	sleep(2000);
	int status = connect(sockid, (struct sockaddr*) &addrserv, sizeof(addrserv));
	printf("Connected to server %d\n", status);

	FILE *fp = fopen("client.test","r");
	uint32_t *c = (uint32_t *)malloc(sizeof(uint32_t)*PAYLOAD_ARR_SIZE);

	send_arr(sockid, fp, c);
	return;
}

int recv_arr(int sockid, FILE *fp, uint32_t *c)
{
	uint32_t *flag;
	uint32_t *d = c;
	long new_size = PAYLOAD_ARR_SIZE*4;
	recv(sockid, c, sizeof(uint32_t)*PAYLOAD_ARR_SIZE,0);
	decapsulate(c,flag);
	if (*flag == DIFF_SIZE){
		new_size = *c;
		recv(sockid,c,new_size,0);
	}else{
		encapsulate(c,flag);
	}
	*flag & 0;
	decapsulate(d+127,flag);
	if (*flag == PAYLOAD){
		puts("Ladies and gentlemen");
		fwrite(c, sizeof(uint32_t),PAYLOAD_ARR_SIZE,stdout);
	}
	return 0;
}

void *launch_server(void *arg)
{
	//Server socket
	struct sockaddr_in addrport, addrcli;
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int s; 
	//Setup socket and ip address structs
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(4414);
	addrport.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//remove any garbage from addrcli memory space
	memset(&addrcli, 0, sizeof(addrcli));
	addrcli.sin_family = AF_INET;
	addrcli.sin_port = htons(INADDR_ANY);
	addrcli.sin_addr.s_addr = htonl(INADDR_ANY);


	bind(sockid,(struct sockaddr *) &addrport,sizeof(addrport));

	int connected = -1;
	int status = listen(sockid, 10);
	puts("Server bound");
	while(connected == -1){
		sleep(2000);
		socklen_t clilen = sizeof(addrcli);		
		s = accept(sockid, (struct sockaddr *) &addrcli, &clilen);
		connected = s;
	}

	FILE *fp = fopen("server.test","w");

	uint32_t *c = (uint32_t *)malloc(sizeof(uint32_t)*PAYLOAD_ARR_SIZE);

	sleep(2000);

	recv_arr(sockid, fp, c);

	return (void *) NULL;
}

void *print_something(void *arg)
{
	puts("Thread!");
	return (void *) NULL;
}

int main()
{
	pthread_t cli_thread, serv_thread, thread_test;
	pthread_create(&thread_test, NULL, print_something, (void *)NULL);
	pthread_create(&cli_thread, NULL, launch_client, (void *)NULL);
	pthread_create(&serv_thread, NULL, launch_server, (void *)NULL);
	puts("Done!");
	pthread_join(cli_thread, NULL);
	pthread_join(serv_thread, NULL);

	return 0;
}
