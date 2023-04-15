/*
 * Hashing algorithm that produces output in hexadecimal of length equal to the length of
 * the buffer times the number of bits in the type of data, in this case, uint32_t
 */

#ifndef HASHR_H
#define HASHR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define LENGTH_BUFFER 4 //in terms of how many data fit inside
#define SHIFT_DISTANCE 1
#define REMAINDER_MASK_LEFT 0x80000000
#define REMAINDER_MASK_RIGHT 0x00000001
#define SIZE_DATA 32

/*
 * Takes a pointer to a buffer, data to be hashed, and a counter for where
 * the data should be offset relative to the significant bits in the buffer
 * Changes the contents of buffer directly and returns no value
 * Alternates between AND and XOR operations and left and right shifts
 */

void hash_uint32(uint32_t *buffer, uint32_t data, uint32_t offset)
{
	uint32_t align = offset % LENGTH_BUFFER;
	uint32_t remainder[LENGTH_BUFFER - 1];
	//The buffer has been iterated an odd number of times
	if ((offset / LENGTH_BUFFER) % 2){
		if (align % 2){
			*(buffer + align) = *(buffer + align) & data;
		}else{
			*(buffer + align) = *(buffer + align) ^ data;
		}
		/*
		 * Shift left, starting at least significant and moving more significant
		 * Collect the remainder of each section of memory and append it to the
		 * more significant block after it has been shifted
		 * Least significant shift distance-bits are truncated
		 */
		for (int i = LENGTH_BUFFER - 1; i >=0; i--){
			if (i > 0){
				//only 3 total remainder values, but 4 i values
				remainder[i - 1] = *(buffer + i) & REMAINDER_MASK_LEFT;
				remainder[i - 1] = remainder[i - 1] >> (SIZE_DATA - 1);
			}
			*(buffer + i) = *(buffer + i) << SHIFT_DISTANCE;
			if (i < (LENGTH_BUFFER - 1)){
				*(buffer + i) = *(buffer + i) | remainder[i];
			}
		}
	}else{
		if (align % 2){
			*(buffer + align) = *(buffer + align) ^ data;
		}else{
			*(buffer + align) = *(buffer + align) & data;
		}
		/*
		 * Shift the result, starting from the most significant 32-bit block
		 * Collect the remainder of each section of memory and append it to the
		 * less significant block after it has been shifted
		 * Most significant shift distance-bits are truncated
		 */
		for (int i = 0; i < LENGTH_BUFFER; i++){
			if (i < LENGTH_BUFFER - 1){
				remainder[i] = *(buffer + i) & REMAINDER_MASK_RIGHT;
				remainder[i] = remainder[i] << (SIZE_DATA - 1);
			}
			*(buffer + i) = *(buffer + i) >> SHIFT_DISTANCE;
			if (i > 0){
				*(buffer + i) = *(buffer + i) | remainder[i - 1];
			}
		}
	}
}

//In bits
#define SIZE_HASH 256
#define SIZE_HASHABLE 32
#define NUM_HASHABLES SIZE_HASH / SIZE_HASHABLE

typedef uint32_t hashable;

const hashable state[64] = {
0x6b4b0edb, 0x1c00a95, 0x65c3176d, 0x16515a60, 0x751043d9, 0x583f7e05, 0x7dfa5797, 0x6edbb516, 
0x7681d412, 0x1bc40a04, 0x26f7a574, 0x72a45cbb, 0x18612bc2, 0xb7cacb, 0x6625d445, 0x20af425e, 
0x74efe862, 0x460d795, 0x3629bf44, 0x787d2ed1, 0x1bb274c5, 0x1bff7917, 0x1ce10274, 0x22956d9a, 
0x317b1c74, 0x5adb3b89, 0x3370c10c, 0x411feaa3, 0x386a47e1, 0x697b1b62, 0x3e2632b4, 0x23b556bc, 
0x6b3b25f7, 0x23e94a21, 0x3a06b11c, 0x604b69d0, 0x7c28c826, 0x380108b3, 0x4f271ee6, 0x72aa9c38, 
0x53c512b7, 0x761ec45b, 0x654ef8f4, 0x6c263e7a, 0x76d68f26, 0x4b74cd39, 0xcd580d8, 0x6bc67788, 
0x4fd5a4ce, 0x42ff401c, 0x6443a659, 0x6b881993, 0x5efeb933, 0x124a8ce, 0xe1d872d, 0x1079d5a8, 
0x5bffe457, 0x418e483a, 0x5199c04b, 0x146a2c38, 0x2b09639c, 0xfbff2ff, 0x381f82f4, 0x16448993	
};

long get_file_size(FILE *fp)
{
	if (fp == NULL)
		return 0;
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return size;
}

char *hash_file(FILE *fp)
{
	//Needs space for string terminator
	char final_hash[SIZE_HASH + 1];
	long file_size = get_file_size(fp);
	long remainder_bytes_size = file_size % SIZE_HASH;
	hashable hash[NUM_HASHABLES];
	
}

#endif
