#include <stdio.h>
#include "hashr.h"
#include <string.h>

int main()
{
	FILE *fp = fopen("../testing/small_test_file", "r");
	FILE *fp_corrupt = fopen("../testing/small_test_file_corrupted", "r");

	printf("%s\tsmall_test_file\n", hash_file(fp));
	printf("%s\tsmall_test_file_corrupted\n", hash_file(fp_corrupt));

	if (strcmp(hash_file(fp), hash_file(fp_corrupt)) == 0){
		printf("Did not find the present corruption\n");
	}else{
		printf("Successfully found corruption\n");
	}

	return 0;
}
