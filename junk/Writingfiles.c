#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void){

	char in[300];
	char inexit[300]="exit";
	int ecomp = 0;
	int echeck = 0;
	char filen[300];

	FILE * fpointer;
	printf("Enter file name:\n");
	scanf("%s", &filen);
	fpointer = fopen(filen, "w");

	printf("Log file created. Enter line or exit:\n");

	FILE * fpointertwo = fopen(filen, "a");

	while(echeck == 0){
		fgets(in, 300, stdin);
		in[strcspn(in, "\n")] = '\0';
		ecomp = strcmp(in, inexit);

		if(ecomp == 0){

		printf("Exiting...\n");
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

	printf("Compile?[y/n]: \n");
	scanf("%c", &reply);

	if(reply == 'y'){
		system(command);
	}else{
		printf("Work complete\n");
	}



	return 0;
}