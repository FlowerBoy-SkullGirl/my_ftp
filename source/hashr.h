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

#endif
