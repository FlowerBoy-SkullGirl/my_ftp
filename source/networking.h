#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include "hashr.h"

#ifndef FTP_NETWORKING_H 
#define FTP_NETWORKING_H

#define MAXLEN 2048
#define PAYLOAD_FLAG 1
#define SIZE_FLAG 2
#define EOF_FLAG 3
#define HASH_FLAG 4
#define PAYLOAD 0x10000000
#define DIFF_SIZE 0x20000000
#define END_FLAG 0x30000000
#define HASH_PAYLOAD 0x40000000
#define INIT_FLAG 0x50000000
#define CORRUPTED_FLAG 0xF0000000
#define REMOVE_FLAG 0x0FFFFFFF
#define EMPTY_DATA 0x00000000
#define DEFAULT_PENDING 1
#define SIZE_CHANGE 2
#define PAYLOAD_SIZE 4 //sizeof(uint32_t)
#define PAYLOAD_ARR_SIZE 128
#define PASS_HANDSHAKE 0
#define FAIL_HANDSHAKE 1
#define FTP_FALSE 0
#define FTP_TRUE 1
#define DEFAULT_PORT 4414
#define PACKET_BYTES (PAYLOAD_SIZE*PAYLOAD_ARR_SIZE)
#define PAYLOAD_BYTES (PAYLOAD_SIZE*(PAYLOAD_ARR_SIZE-1))

//Struct of inet_addr for reference
/*	unsigned int inet_addr(char *str){
		int a, b, c, d;
		char arr[4];
		sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
		arr[0] = a; arr[1] = b, arr[2] = c, arr[3] = d;
		return *(unsigned int *)arr;
	}
*/	

struct ftp_sock{
	short port;
	int max_pending_connections;
};

//Separates data and flag, returns data
uint32_t *decapsulate(uint32_t *data, uint32_t *flag)
{
	*flag = 0xF0000000 & *data;
	*data = *data & REMOVE_FLAG;
	return data;
}

//Confirms synchronization at the application layer
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

//Initializes transfer of info, waits until server is ready for filename, 0 on success	
int handshake_client(int sockid, char *sended){
	//Servers success signal is 1	
	//Assign the init flag in network order
	uint32_t message = htonl(INIT_FLAG);
	
	//Send a hello and receive the success signal
	send(sockid, &message, sizeof(uint32_t), 0);
	//overwrite message to ensure that the data received next is new
	message = message & EMPTY_DATA;
	recv(sockid, &message, sizeof(uint32_t), 0);
	
	//Make sure byte order
	message = ntohl(message);
	
	//Verify success	
	if(message == INIT_FLAG){
		puts("Server returned init signal");
		return PASS_HANDSHAKE;
	}else{
		fprintf(stderr, "Server did not return signal");
		return FAIL_HANDSHAKE;
	}
}

//Reserve the 1st 4 bits for type of data info, last 24 for payload (4 currently unused, in same byte as info flags)
uint32_t encapsulate(char type, uint32_t data)
{
	if (type == PAYLOAD_FLAG){
		data = data | PAYLOAD;
	}else if (type == SIZE_FLAG){
		data = data | DIFF_SIZE;
	}else if (type == EOF_FLAG){
		data = data | END_FLAG;
	}else if (type == HASH_FLAG){
		data = data | HASH_PAYLOAD;
	}else{
		return data | CORRUPTED_FLAG;
	}
	
	return data;
}

#endif
