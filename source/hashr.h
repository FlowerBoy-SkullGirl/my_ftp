/*
 * Hashing algorithm that produces output in hexadecimal of length equal to the length of
 * the buffer times the number of bits in the type of data, in this case, uint32_t
 */

#ifndef HASHR_H
#define HASHR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//In bits
#define SIZE_HASH 256
#define SIZE_HASH_BYTES SIZE_HASH / 8
#define SIZE_HASHABLE 32
#define SIZE_HASHABLE_BYTES SIZE_HASHABLE / 8
#define NUM_HASHABLES SIZE_HASH / SIZE_HASHABLE
#define CHAR_HEX_TO_BYTES 2
#define OFF 0
#define ON 1
#define COMPRESSION_ROUNDS 32
#define LEFTMOST_3_BITS   0xE0000000
#define RIGHTMOST_29_BITS 0x1FFFFFFF
#define LEFTMOST_28_BITS  0xFFFFFFF0 
#define RIGHTMOST_4_BITS  0x0000000F
#define LEFT_SHIFT_LENGTH 3
#define LEFT_SHIFT_ADJUST_R SIZE_HASHABLE - LEFT_SHIFT_LENGTH
#define RIGHT_SHIFT_LENGTH 4
#define RIGHT_SHIFT_ADJUST_L SIZE_HASHABLE - RIGHT_SHIFT_LENGTH

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

void initialize_state(hashable *hash_state)
{
	for (int i = 0; i < NUM_HASHABLES; i++){
		*(hash_state + i) = state[i];
	}
}

void compress_state_and_data(hashable *hash_state, hashable *f_data)
{
	hash_state[0] = hash_state[0] | (f_data[0] | (f_data[5] ^ state[20]));
	hash_state[1] = hash_state[1] ^ (f_data[1] & ((f_data[3] | state[18])));
	hash_state[2] = hash_state[2] ^ ~(f_data[2]);
	hash_state[3] = hash_state[3] | ~((f_data[3] & state[12]) | f_data[6]);
	hash_state[4] = hash_state[4] & ~(f_data[4]);
	hash_state[5] = hash_state[5] & ((f_data[5] ^ state[15]) & f_data[0]);
	hash_state[6] = hash_state[6] | (f_data[6] | (f_data[4] & state[13]));
	hash_state[7] = hash_state[7] ^ (f_data[7] & ((f_data[1] | state[8]) ^ (f_data[4] ^ state[16])));
}

void compress_state_and_data2(hashable *hash_state, hashable *f_data)
{
	hash_state[0] = hash_state[0] | (f_data[0] | ((f_data[6] ^ state[20]) & (f_data[2] & state[19])));
	hash_state[1] = hash_state[1] ^ (f_data[1] & ((f_data[3] | state[18] ^ f_data[7])));
	hash_state[2] = hash_state[2] ^ ~(f_data[2] | (f_data[4] & state[32]));
	hash_state[3] = hash_state[3] | ~((f_data[3] & state[12]) | f_data[6] ^ f_data[4]);
	hash_state[4] = hash_state[4] & ~((f_data[4]) | (f_data[7] & f_data[0] ^ state[53]));
	hash_state[5] = hash_state[5] & ((f_data[5] ^ state[15]) & (f_data[0] ^ f_data[2]));
	hash_state[6] = hash_state[6] | ((f_data[6] & f_data[1]) | (f_data[4] & state[13]));
	hash_state[7] = hash_state[7] ^ (f_data[7] & ((f_data[1] | state[8]) ^ (f_data[5] ^ state[16])));
}


void scramble1(hashable *hash_state)
{ 
	hashable temp[NUM_HASHABLES];
	for (int i = 0; i < NUM_HASHABLES; i++)
		temp[i] = hash_state[i];

	hash_state[0] = temp[2];
	hash_state[1] = temp[7];
	hash_state[2] = temp[3];
	hash_state[3] = temp[1];
	hash_state[4] = temp[5];
	hash_state[5] = temp[0];
	hash_state[6] = temp[4];
	hash_state[7] = temp[6];
}

void scramble2(hashable *hash_state)
{ 
	hashable temp[NUM_HASHABLES];
	for (int i = 0; i < NUM_HASHABLES; i++)
		temp[i] = hash_state[i];

	hash_state[0] = temp[3];
	hash_state[1] = temp[0];
	hash_state[2] = temp[4];
	hash_state[3] = temp[1];
	hash_state[4] = temp[6];
	hash_state[5] = temp[3];
	hash_state[6] = temp[7];
	hash_state[7] = temp[2];
}

void shift_state_left(hashable *hash_state)
{
	hashable temp[NUM_HASHABLES * 2];
	
	int j = 0;
	for (int i = 0; i < NUM_HASHABLES; i++){
		temp[j++] = (hash_state[i] & LEFTMOST_3_BITS) >> LEFT_SHIFT_ADJUST_R;
		temp[j++] = (hash_state[i] & RIGHTMOST_29_BITS) << LEFT_SHIFT_LENGTH;
	}
	//Each hash will be composed of its rightmost bits shifted left and appended with its neighbors lefmost bits
	j = 1;
	for (int i = 0; i < NUM_HASHABLES - 1; i++){
		hash_state[i] = temp[j] | temp[j+1];
		j+=2;
	}
	//The last hash will borrow bits from the first hash instead of its neighbor
	hash_state[NUM_HASHABLES - 1] = temp[(NUM_HASHABLES * 2) - 1] | temp[0];
}

void shift_state_right(hashable *hash_state)
{	
	hashable temp[NUM_HASHABLES * 2];
	
	int j = 0;
	for (int i = 0; i < NUM_HASHABLES; i++){
		temp[j++] = (hash_state[i] & LEFTMOST_28_BITS) >> RIGHT_SHIFT_LENGTH;
		temp[j++] = (hash_state[i] & RIGHTMOST_4_BITS) << RIGHT_SHIFT_ADJUST_L;
	}
	//Each hash will be composed of its leftmost bit shifted right and appended with its neighbors rightmost bits
	//The first hash will borrow bits from the last hash instead of its neighbor
	hash_state[0] = temp[0] | temp[(NUM_HASHABLES * 2) - 1];
	j = 1;
	for (int i = 1; i < NUM_HASHABLES; i++){
		hash_state[i] = temp[j] | temp[j+1];
		j+=2;
	}
}

void pad_data(hashable *file_data, long remainder_size)
{
	long size_initialized = (SIZE_HASHABLE_BYTES * NUM_HASHABLES) - remainder_size;
	if (size_initialized < (SIZE_HASHABLE_BYTES * (NUM_HASHABLES - 1)))
		file_data[NUM_HASHABLES - 1] = (hashable) remainder_size;
}

char *hash_file(FILE *fp)
{
	//Needs space for string terminator
	char *final_hash = (char *) malloc((SIZE_HASH_BYTES * CHAR_HEX_TO_BYTES) + 1);
	long file_size = get_file_size(fp);
	long remainder_bytes_size = file_size % (SIZE_HASHABLE_BYTES * NUM_HASHABLES);
	hashable hash[NUM_HASHABLES];
	hashable file_data[NUM_HASHABLES];
	int shuffle_state = OFF;
	
	initialize_state(hash);
	
	while ((file_size - ftell(fp)) > remainder_bytes_size){
		fread(file_data, SIZE_HASHABLE_BYTES, NUM_HASHABLES, fp);
for(int i = 0; i < COMPRESSION_ROUNDS; i++){
		compress_state_and_data2(hash, file_data);
		if (shuffle_state == ON){
			shift_state_left(hash);
			scramble2(hash);
			shuffle_state = OFF;
		}else{
			shift_state_right(hash);
			scramble1(hash);
			shuffle_state = ON;
		}
}
	}
	for (int i = 0; i < NUM_HASHABLES; i++)
		file_data[i] = 0;

	fread(file_data, 1, remainder_bytes_size, fp); 
	pad_data(file_data, remainder_bytes_size);

for(int i = 0; i < COMPRESSION_ROUNDS; i++){
	compress_state_and_data2(hash, file_data);
	if (shuffle_state == ON){
		shift_state_left(hash);
		scramble2(hash);
		shuffle_state = OFF;
	}else{
		shift_state_right(hash);
		scramble1(hash);
		shuffle_state = ON;
	}
	
}
	snprintf(final_hash, (SIZE_HASH_BYTES * CHAR_HEX_TO_BYTES) + 1, "%08x%08x%08x%08x%08x%08x%08x%08x", hash[0], hash[1],
	                          hash[2], hash[3], hash[4], hash[5], hash[6], hash[7]);
	return final_hash;
}

#endif
