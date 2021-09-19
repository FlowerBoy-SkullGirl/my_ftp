#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void new(void){
	

	char listname[300]="list.txt";
	int reply;

	printf("Creating new list. This will overwrite any old list. Continue? 1. Yes 2. No\n");
	scanf("%d", &reply);

	if(reply==1){

	FILE * nf;
	nf = fopen(listname, "w");

	printf("Enter single items and then hit enter. When done, type exit and enter\n");

	int donecheck=1;
	char item[300];
	char exitcheck[300]="exit";
	char empty[300];
	//attempt to remove empty first item
	fgets(empty, 300, stdin);

	while(donecheck!=0){

		fgets(item, 300, stdin);
		item[strcspn(item, "\n")] = '\0';
		donecheck=strcmp(item, exitcheck);

		if(donecheck==0){
			printf("Exiting...\n");
		}else{

			fprintf(nf, "%s\n", item);
		}

	}
		fclose(nf);
		printf("Returning to main menu...\n");
		}else{
		printf("Exiting to main menu without writing...\n");
				}
}