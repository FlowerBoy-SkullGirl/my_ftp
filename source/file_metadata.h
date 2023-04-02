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
	return (uint32_t) ((x & 0xFFFFFFFF00000000) >> sizeof(uint32_t));	
}

uint32_t last_sig_byte_long(long x)
{
	return (uint32_t) (x & 0x00000000FFFFFFFF);	
}

char *truncate_file_path(char *filen_full)
{
	char *filenout = (char *)malloc(strlen(filen_full)+1);
	strcpy(filenout, filen_full);
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
	
	return filenout;
}


/*
 * Allocates memory for filename that must be freed. Returns a metadata struct pointer
 * Takes file info IN and structured info OUT
 */
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

//Used as a destructor for any metadata pointers and the memory allocated for the file name. Should always be called instead of simply free.
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
	*packet = META_FLAG;// | session_mask;
	//temporarily disabled while session id not in use
	uint32_t *packet_contents = packet;
	char *packet_string;
	char *name = data.name;

	if (sizeof(long) == sizeof(uint32_t)){
		*(++packet_contents) = 0;
		*(++packet_contents) = data.size;
		*(++packet_contents) = 0;
		*(++packet_contents) = data.name_size;
		//cast uint32 *memory to char *
		packet_string = (char *) packet_contents;
		for (int i = 0; i < data.name_size; i++){
			*(++packet_string) = *name;
			name++;
		}
		return PACKET_BYTES;
	//Most likely case is long as 64 bit integer, which must be broken into 32 bit segments.
	}else if (sizeof(long) == (2 * sizeof(uint32_t))){
		*(++packet_contents) = first_sig_byte_long(data.size);
		*(++packet_contents) = last_sig_byte_long(data.size);
		*(++packet_contents) = first_sig_byte_long(data.name_size);
		*(++packet_contents) = last_sig_byte_long(data.name_size);
		packet_string = (char *) (++packet_contents);
		for (int i = 0; i < data.name_size; i++){
			*(packet_string) = *name;
			packet_string++;
			name++;
		}
		*packet_string = '\0';
		return PACKET_BYTES;
	}
	return FTP_FALSE;
}

/*
 * Takes a packet that has been read containing metadata and deserializes it into a metadata struct
 */
struct metadata *pack_metadata_packet(uint32_t *packet)
{
	struct metadata *file_data = (struct metadata *) malloc(sizeof(struct metadata));
	//cast uint memory as char *
	char *filen =(char *) (packet + OFFSET_FILE_NAME);

	//FILE SIZE
	file_data->size = file_data->size | (*(packet + OFFSET_FILE_SIZE) << sizeof(uint32_t));
	file_data->size = file_data->size | *(packet + OFFSET_FILE_SIZE + 1);
	//FILE NAME SIZE
	file_data->name_size = file_data->name_size | (*(packet + OFFSET_FILE_NAME_SIZE) << sizeof(uint32_t));
	file_data->name_size = file_data->name_size | *(packet + OFFSET_FILE_NAME_SIZE + 1);

	//ALLOC MEMORY FOR STRING NAME
	file_data->name = (char *) malloc(file_data->name_size);
	char *temp = file_data->name;

	for (int i = 0; i < file_data->name_size; i++){
		*temp = *filen;
		temp++;
		filen++;
	}
	*temp = '\0';
	return file_data;
}

#endif
