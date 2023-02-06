#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "hashr.h" //Hash algorithm for validating file integrity
#include "networking.h" //Defines for flags. Functions for send and receive

int recv_arr(int sockid, FILE *fp, uint32_t *c)
{
	uint32_t *flag;
	long new_size = PAYLOAD_ARR_SIZE*sizeof(uint32_t);
	while(*flag != PAYLOAD && *flag != END_FLAG){
		int size_recv = recv(sockid, c, PAYLOAD_ARR_SIZE*PAYLOAD_SIZE,0);
		fwrite(c, sizeof(uint32_t),PAYLOAD_ARR_SIZE,stdout);
		printf("Data: %x\n", *(c));
		printf("Data 1: %x\n", *(c+1));
		printf("Data 2: %x\n", *(c+2));
		printf("Size recv: %x\n", size_recv);
		perror("yer");
		decapsulate(c,flag);
		printf("Flag: %x\n", *flag);

		if (*flag == DIFF_SIZE){
			puts("Gets diff");
			new_size = *c;
			printf("New size %d\n",new_size);
		}
		if (*flag == PAYLOAD || *flag == END_FLAG){
			puts("Ladies and gentlemen");
			fwrite((c+1), sizeof(uint32_t),PAYLOAD_ARR_SIZE-1,stdout);
			fwrite((c+1), sizeof(uint32_t),new_size,stdout);
			fwrite((c+1), 1,new_size,fp);
			printf("Data 3 & 4: %x %x\n", *(c+3), *(c+4));
		}
		*flag & 0;
	}
	puts("sent");
	send(sockid,c,PAYLOAD_ARR_SIZE*sizeof(uint32_t),0);
	return 0;
}

int main()
{
	//Server socket
	struct sockaddr_in addrport, addrcli;
	int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int s = -1; 
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

	int status = listen(sockid, 10);
	puts("Server bound");
	while(s == -1){
		socklen_t clilen = sizeof(addrcli);		
		s = accept(sockid, (struct sockaddr *) &addrcli, &clilen);
	}

	FILE *fp = fopen("server.test","w");

	uint32_t *c = (uint32_t *)malloc(sizeof(uint32_t)*PAYLOAD_ARR_SIZE);

	recv_arr(s, fp, c);

	return 0;
}


