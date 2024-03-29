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

//VERSION CONTROL
#define STANDARD_VERSION_MAJ 1
#define STANDARD_VERSION_MIN 0

/*
 * The version packet must always be the same across versions, otherwise backwards compatibility will be broken
 * -----------------
 * 4 byte Session start flag, 4 byte Major Version, 4 byte minor version, 500 bytes reserved
 * -----------------
 * The reserved bytes are there to simplify the logic of the program, despite inefficiency.
 * It is easier to use the same buffer size for all data rather than write new code for each
 * data that needs to be sent.
 */

#define MAXLEN 2048

#define PAYLOAD 0x10000000
#define DIFF_SIZE 0x20000000
#define EOF_FLAG 0x30000000
#define HASH_PAYLOAD 0x40000000
#define SESSION_ID_FLAG 0x50000000
#define META_FLAG 0x60000000
#define END_SESSION 0x70000000
#define RETRY_FLAG 0x8000000
#define SESSION_START 0x90000000
#define NEXT_FLAG 0xA0000000
#define CORRUPTED_FLAG 0xF0000000
#define REMOVE_FLAG 0x0FFFFFFF
#define EMPTY_DATA 0x00000000

#define VERSION_MISMATCH 0x0000000F
#define MISSING_DATA 0x000000F0
#define UNEXPECTED_FLAG 0x00000F00

#define DEFAULT_PENDING 1
#define SIZE_CHANGE 2
#define PAYLOAD_SIZE 4 //sizeof(uint32_t)
#define PAYLOAD_ARR_SIZE 128
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

//Generic function to read a specified buffer size, returns amount read
uint32_t read_buffer(int s, uint32_t *buffer, long buffer_size)
{	
	long size_read = 0;
	while (size_read < buffer_size){
		size_read += recv(s, buffer + (size_read/PAYLOAD_SIZE), buffer_size - size_read, 0);
	}
	return size_read;
}

//Sends an end session signal with an accompanying error message. 
uint32_t fatal_error_hangup(int sock, uint32_t *buffer, long buffer_size, uint32_t error_flag)
{
	memset(buffer, 0, buffer_size);
	*buffer = END_SESSION;
	*(buffer + 1) = error_flag;
	send(sock, buffer, buffer_size, 0);
	return FTP_FALSE;
}

void build_hash_packet(char *hash_str, uint32_t *packet, int session_mask)
{
	*packet = HASH_PAYLOAD; /* | session_mask; */

	int strlen_hash = (strlen(hash_str) + 1);
	*(++packet) = strlen_hash;

	//Cast packet to a char * to copy the rest of the hash
	packet++;
	char *packet_char = (char *) packet;

	for (int i = 0; i < strlen_hash; i++){
		*(packet_char) = *hash_str;
		packet_char++;
		hash_str++;
	}
	*packet_char = '\0';
}

char *unpack_hash_packet(uint32_t *packet)
{
	//Hash length should be 256 bits, but represented in hex is 64 bytes + the null terminator
	int hash_str_length = *(++packet);

	//Cast packet to char *
	(++packet);
	char *packet_str = (char *) packet;
	char *hash_str = (char *) malloc(hash_str_length);

	for (int i = 0; i < hash_str_length; i++){
		*(hash_str + i) = *(packet_str + i);
	}
	*(hash_str + (hash_str_length - 1)) = '\0';
	return hash_str;
}

#endif
