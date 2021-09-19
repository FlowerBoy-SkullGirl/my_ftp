#include <stdio.h>
#include <string.h>

	void rem(void){

		char listname[300]="list.txt";
		char rlines[300][300];
		int rhelp=0;
		int rhelpfin=0;
		char emptyl[300]="\n";

		FILE *rem;
		rem =fopen(listname, "r");

	
		if(rem ==NULL){
			printf("Error, rem returned null\n");
		}else{


		while( fgets(rlines[rhelp], 300, rem) != NULL && rhelp<300 ){
			rhelp++;
		}


		fclose(rem);

	} //safety NULL check
		rhelpfin=rhelp;

		rem = fopen(listname, "w");

		for(rhelp=0;rhelp<rhelpfin;rhelp++){
			if( strcmp(rlines[rhelp], emptyl) !=0){
				fprintf(rem, "%s", rlines[rhelp]);
			}else if( strcmp(rlines[rhelp], emptyl) ==0 && rhelp+1>=rhelpfin){
				rhelpfin--;
			}

		}

		fclose(rem);

	}