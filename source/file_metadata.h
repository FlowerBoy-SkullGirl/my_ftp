#include "networking.h"

#ifndef FTP_META_H
#define FTP_META_H

//Offsets are measured in 32-bit intervals from start of packet
#define OFFSET_FILE_SIZE 1
#define OFFSET_FILE_NAME_SIZE 3
#define OFFSET_FILE_NAME 5

/* Structure of a metadata packet
 * 1 Byte Flag, 1 Byte session id, 2 bytes reserved
 * 8 Bytes file size
 * 8 Bytes file name size
 * Remainder 492 Bytes reserved for file name
 */


struct metadata{
	long size;
	long name_size;
	char *name;
};

uint32_t first_sig_byte_long(long x)
{
	return (uint32_t) ((x & 0xFFFFFFFF00000000) >> sizeof(uint32_t))	
}

uint32_t last_sig_byte_long(long x)
{
	return (uint32_t) (x & 0x00000000FFFFFFFF)	
}

//Allocates memory for filename that must be freed. Returns a metadata struct pointer
struct metadata *pack_file_data(FILE *fp, char *filen)
{
	struct metadata *fp_data;
	fp_data = (struct metadata *) malloc(sizeof(struct metadata));
	fseek(fp, 0, SEEK_END);
	fp_data->size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	fp_data->name_size = strlen(filen) + 1;
	fp_data->name = (char *) malloc(strlen(filen)+1);
	strcpy(fp_data->name, filen);

	return fp_data;
}

int free_metadata(struct metadata *data)
{
	if (data == NULL)
		return FTP_TRUE;
	if (data->name != NULL){
		free(data->name);
	}
	free(data);
	return FTP_TRUE;
}

/*
 * Takes a metadata struct, a memory buffer, and a session id 
 * and builds a packet that holds the metadata of the file to be sent. 
 * Returns the size of the memory buffer.
 * Returns FTP_FALSE or 0 on fail.
 */
int build_metadata_packet(struct metadata data, uint32_t *packet, uint32_t session_mask)
{
	memset(packet,0,PACKET_BYTES);
	*packet = META_FLAG | session_mask;
	uint32_t *packet_contents = packet;
	char *packet_string;

	if (sizeof(long) == sizeof(uint32_t)){
		*(++packet_contents) = 0;
		*(++packet_contents) = data.size;
		*(++packet_contents) = 0;
		*(++packet_contents) = data.name_size;
		packet_string = packet_contents;
		for (int i = 0; i < data.name_size; i++){
			*(++packet_string) = *name;
			name++;
		}
		return PACKET_BYTES;
	}else if (sizeof(long) == (2 * sizeof(uint32_t))){
		*(++packet_contents) = first_sig_byte_long(data.size);
		*(++packet_contents) = last_sig_byte_long(data.size);
		*(++packet_contents) = first_sig_byte_long(data.name_size);
		*(++packet_contents) = last_sig_byte_long(data.name_size);
		packet_string = packet_contents;
		for (int i = 0; i < data.name_size; i++){
			*(++packet_string) = *name;
			name++;
		}
		return PACKET_BYTES;
	}
	return FTP_FALSE;
}

struct metadata *pack_metadata_packet(uint32_t *packet)
{
	struct metadata *file_data = (struct metadata *) malloc(sizeof(struct metadata));
	char *filen = packet + OFFSET_FILE_NAME;

	//FILE SIZE
	file_data->size = file_data->size | (*(packet + OFFSET_FILE_SIZE) << sizeof(uint32_t));
	file_data->size = file_data->size | *(packet + OFFSET_FILE_SIZE + 1);
	//FILE NAME SIZE
	file_data->name_size = file_data->name_size | (*(packet + OFFSET_FILE_NAME_SIZE) << sizeof(uint32_t));
	file_data->name_size = file_data->name_size | (*(packet + OFFSET_FILE_NAME_SIZE + 1) << sizeof(uint32_t));

	//ALLOC MEMORY FOR STRING NAME
	file_data->name = (char *) malloc(file_data->name_size);

	for (int i = 0; i < file_data->name_size; i++){
		*(file_data->name) = *filen;
		file_data->name++;
		filen++;
	}
	return file_data;
}

#endif
