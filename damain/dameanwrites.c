#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void write(void){

	char in[300];
	char inexit[300]="exit";
	int ecomp = 0;
	int echeck = 0;
	char filen[300];

	FILE * fpointer;
	printf("\nEnter file name(specify file type): ");
	scanf("%s", &filen);
	fpointer = fopen(filen, "w");

	printf("File created. Enter line or type 'exit':\n");

	FILE * fpointertwo = fopen(filen, "a");

	while(echeck == 0){
		fgets(in, 300, stdin);
		in[strcspn(in, "\n")] = '\0';
		ecomp = strcmp(in, inexit);

		if(ecomp == 0){

		printf("\nFile saved as: %s\n", filen);
		echeck = 1;
		}
		else{

		fprintf(fpointertwo, "%s\n", in);
		}

	}

	fclose(fpointer);
	fclose(fpointertwo);

	char reply;
	char fileno[300];
	strcpy(fileno, filen);
	fileno[strcspn(fileno, ".c")] = '\0';

	char command[300];
	strcpy(command, "gcc ");
	strcat(command, filen);
	strcat(command, " -o ");
	strcat(command, fileno);


	printf("Compile?[y/n]: ");
	scanf("%c", &reply);

	if(reply == 'y'){
		system(command);
		printf("Compiled as: %s\nWork complete\n", fileno);
	}else{
		printf("Work complete. Returning...\n\n");
	}

}