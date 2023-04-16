#include <stdio.h>
#include "hashr.h"

int main()
{
	FILE *fp = fopen("testing/small_test_file", "r");
	FILE *fp_corrupt = fopen("testing/small_test_file_corrupted", "r");

	printf("Hash: %s\n", hash_file(fp));
	printf("Hash corrupt: %s\n", hash_file(fp_corrupt));

	return 0;
}
