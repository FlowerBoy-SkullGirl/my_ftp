#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void check(void){


	char listname[300]="list.txt";
	char clist[300]="clist.txt";
	char clisttest[300]="clisttest.txt";
	
	int aliner=0;
	char clines[300][300];
	int compcheck=0;
	int compr=0;
	char rlines[300][300];
	char nlines[300][300];
	int skipcheck=0;
	int alineradj;

//make sure check file exists to read without overwriting an existing file
	FILE * checky;
	checky = fopen(clist, "a");
	fclose(checky);

	//first check if complete
	FILE * cl;
	cl = fopen(clist, "r");

	if(cl==NULL){
		printf("Error, list returned null\n");
	}else{


	//fills array clines with values in clist.txt
	while(fgets(clines[aliner], 300, cl) !=NULL ){
		//for debugging
		//printf("TEST: what is here %s %d\n", clines[aliner], aliner);
		if( strcmp(clines[aliner], "0\n") ==0 ){
			compcheck=1;
		}
		aliner++;

	}
		if(compcheck ==0){                                            //I might
			printf("List has already been completed today!\n");       // remove this
		}

	fclose(cl);
	} //this is my safety check


	alineradj=aliner;
	aliner=0;

	//calls earlier check to enter loop for checking items
	while(compcheck!=0){
		printf("List is not complete, do you want to complete items? 1. Yes 2. No\n");
		scanf("%d", &compr);

		if(compr ==2){
			break;
		}
		if(compr==1){
			printf("Press 1 if complete or 2 to skip:\n");
			FILE * chr;
			chr = fopen(listname, "r");


			if(chr==NULL){
				printf("Error, chr returned null\n");
			}else{


			while(fgets(rlines[aliner], 300, chr) != NULL ){
				if( strcmp(clines[aliner], "0\n") ==0 ){ //checks if item is already complete and does not display it
				printf("%s", rlines[aliner]);
				compr=0;

				scanf("%d", &compr);
				if(compr==2){
				skipcheck=1;
				strcpy(nlines[aliner], "0\n");

				//testing debugging remove
				//printf("Line %d value is %s\n", aliner, nlines[aliner]);
				}
				if(compr==1){
				strcpy(nlines[aliner], "1\n");

				//testing debugging remove
				//printf("Line %d value is %s\n", aliner, nlines[aliner]);
				}
				}
				if( strcmp(clines[aliner], "1\n") == 0){   // double checks that items that were
					strcpy(nlines[aliner], clines[aliner]); // already done are marked done
				}
				aliner++;
			}
			if(skipcheck==1){
				printf("Not all list items were completed. Check again? 1. Yes 2. No\n");
			}

			fclose(chr);

			} //This is my safety checker



			aliner=0;

			FILE * doner;
			doner= fopen(clist, "w");

			while(aliner<alineradj){
				fprintf(doner, "%s", nlines[aliner]);
				aliner++;
			}
			fclose(doner);
		}
		compcheck=0; //just testing
		aliner=0;
		compr=0;
		if(skipcheck==1){
		scanf("%d", &compr);
		
		if(compr==1){
		//redo this part to send user back to loop
			cl= fopen(clist, "r");

			if(cl==NULL){
				printf("Error, cl returned null\n");
			}else{

	//fills array clines with values in clist.txt
	while(fgets(clines[aliner], 300, cl) !=NULL ){
		//for debugging
		//printf("TEST: what is here %s %d\n", clines[aliner], aliner);
		if( strcmp(clines[aliner], "0\n") ==0 ){
			compcheck=1;
		}
		aliner++;

											}
		fclose(cl);

		} //Safety check for file

		aliner=0; //reset so it can be looped again

		 }else{
		 	skipcheck=0;
		 } //this is my reloop checker if
		} //this makes sure you skipped before asking for an input

	} //this is the checking part while loop

} //this is void