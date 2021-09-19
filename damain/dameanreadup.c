#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct reader{

char line[300];
int which;	
};



void read(void){

	int done = 0;
	char filen[300];

	printf("\nSpecify file to open: ");
	scanf("%s", &filen);
	
	FILE * freader = fopen(filen, "r");

	fseek(freader, 0, SEEK_END);
	long size = ftell(freader);
	fseek(freader, 0, SEEK_SET);

	char * mem = malloc(size + 1);
	fread(mem, 1, size, freader);

	printf("%s", mem);
	mem[size] = 0;
	free(mem);

	fclose(freader);

	printf("\nDone reading file %s. Returning...\n\n", filen);
		}