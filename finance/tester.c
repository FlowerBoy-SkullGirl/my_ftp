#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ABIGNEGNUM -999999999999
#define MAXSTR 255

void my_charcat(char *dest, char *src){
	while(*dest != '\0'){
		*dest++;
	}
	*dest++ = *src;
	*dest = '\0';

}

void my_strcat(char *dest, char *src){
	while(*dest != '\0'){
		*dest++;
	}
	while(*src != '\0'){
		*dest++ = *src++;
	}
	*dest = '\0';
}


char *usrnums(char *dest){
	char input[MAXSTR];
	char equal[MAXSTR] = "=";


	printf("No current data for: %s\nPlease input data: \n", dest);
	scanf("%s", &input);


	my_strcat(dest, equal);
	my_strcat(dest, input);
}
	void buffs(char *filen, char *src, FILE *fp){
		char rlines[MAXSTR][MAXSTR];
		int rhelp=0;
		int rhelpfin=0;
		char emptyl[MAXSTR]="\n";
		char equal[MAXSTR] = "=";
		char temp[MAXSTR];
		char templ[MAXSTR];

		//Make a copy of setting and an additional copy with a newline character
		strcpy(temp, src);
		temp[strcspn(src, "=")] = 0;
		my_strcat(temp, equal);
		printf("%s", temp);
		strcpy(templ, temp);
		my_strcat(templ, emptyl);

		FILE *buffs = fp;
		fseek(fp, 0, SEEK_SET);
	
		if(buffs ==NULL){
			printf("Error, buffs returned null\n");
		}else{


		while( fgets(rlines[rhelp], MAXSTR, buffs) != NULL && rhelp<MAXSTR ){
			printf("%s", rlines[rhelp]);
			if( strcmp(temp, rlines[rhelp]) ==0 ){
				puts("it does that");
				strcpy(rlines[rhelp], src);
			}else if(strcmp(templ, rlines[rhelp])==0){
					puts("it does this");
					my_strcat(src, emptyl);
					strcpy(rlines[rhelp], src);
					}
			rhelp++;
		}


			} //safety NULL check
		rhelpfin=rhelp;

		buffs = fopen(filen, "w");

		for(rhelp=0;rhelp<rhelpfin;rhelp++){
			if( strcmp(rlines[rhelp], emptyl) !=0){
				fprintf(buffs, "%s", rlines[rhelp]);

				}else if( strcmp(rlines[rhelp], emptyl) ==0 && rhelp+1>=rhelpfin){
					rhelpfin--;
			}
		}

	}

int main(){
	char destination[MAXSTR]="current_balance";
	char source[MAXSTR];
	char *dest = destination;
	char *src = source;
	char filen[MAXSTR] = "test.txt";
	char line[MAXSTR];
	int curfp = 0;
	int sizeln = 0;
	char setting[MAXSTR] = "current_balance=556.73";
	char emptyl[MAXSTR]="\n";
	char testscr[MAXSTR]="Hello\n";

	FILE *fp = fopen(filen, "r+");
	fgets(line, MAXSTR, fp);
	curfp = ftell(fp);
	sizeln = strlen(line);


	printf("Test value strcspn: %d", strcspn(testscr, emptyl));



	fclose(fp);
	return 0;
}


