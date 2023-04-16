#include <stdio.h>
#include "hashr.h"
#include <string.h>

#define MAX 256

int main()
{
	char filen[MAX] = "../testing/small_test_file";
	char filen_corrupt[MAX] = "../testing/small_test_file_corrupted";
	FILE *fp = fopen(filen, "r");
	FILE *fp_corrupt = fopen(filen_corrupt, "r");

	printf("%s\ttest file\n", hash_file(fp));
	printf("%s\ttest file corrupted\n", hash_file(fp_corrupt));

	if (strcmp(hash_file(fp), hash_file(fp_corrupt)) == 0){
		printf("Did not find the present corruption\n");
	}else{
		printf("Successfully found corruption\n");
	}

	return 0;
}
