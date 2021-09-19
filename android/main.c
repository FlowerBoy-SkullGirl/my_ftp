#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRSIZE 32
#define STCTSIZE 2
#define SIGDIG 2
#define MAXD 256	
#define FILENAME "financr_info.txt"

	//Flush getchar
	void flush(){
		int c;
		while(c=getchar() != '\n' && c != EOF);
	}

	char *my_reverse(char *dest){
		char *temp;
		int i = 0;
		int y = 0;
		
		//Malloc for temp
		temp = malloc(MAXD+1);

		//Go to end of str
		while( (*(dest+i) != '\0' ) ){
			i++;
		}
		//Go back one
		i--;

		//Fill temp with dest in reverse order
		while(i>=0){
			*(temp+y) = *(dest+i);
			y++;
			i--;
		} 
		
		//Add \0
		*(temp+y) = '\0';
		
		//Now implement a strcpy
		i = 0;
	
		while( *(temp+i) != '\0' ){
			*(dest+i) = *(temp+i);
			i++;
		}
		
		*(dest+i) = '\0';
		
		free(temp);	
		
		return dest;
	}
	
	char *my_ftoa(double src, char *dest){
		int temp = 0;
		double final = 0;
		int i = 0;
		int negativeflag = 0;

		//Make src positive and raise a flag if it was negative
		if(src<0){
			negativeflag = 1;
			src *= -1;
		}

		temp = 100 * src;	
		
		//Push number of sig digits onto stack		
		while(i<SIGDIG){
			*(dest+i) = ((temp%10) + '0');
			temp /= 10;
			i++;
		}
		//Push decimal on stack
		*(dest+i) = '.';
		i++;

		//Push remaining numbers on stack
		while(temp>0){
			*(dest+i) = ((temp%10) + '0');
			temp /= 10;
			i++;
		}
		
		//Push '-' onto stack if negative flag
		if(negativeflag==1){
			*(dest+i) = '-';
			i++;
		}
		
		//Signal end of string
		*(dest+i) = '\0';

		//Reverse string
		my_reverse(dest);

		return dest;
	}

	//User info struct
	struct usr{
		double debit_bal;
		double credit_bal;
		double saving;
	};

	//Create a text file with doubles balance for each item
	//Use this text file to fill user info struct
	struct usr *fileget(){
		struct usr *np;
		char filen[STRSIZE] = "financr_info.txt";
		char buff[STCTSIZE][MAXD];
		int i = 0;
		//Alloc space for struct
		np = malloc(sizeof(struct usr));
		
		FILE * fp;
		
		fp = fopen(filen, "r");
		//If file doesn't exist, or is null		
		if(fp == NULL){
			fp = fopen(filen, "w");	
			
			i=0;
			while(i<=STCTSIZE){
				fputs("0\n", fp);
				i++;
			}
			fclose(fp);
						
			fp = fopen(filen, "r+");
			if(fp == NULL){
				fputs("File bad\n", stderr);
				return np;
			}
		}
		
		//Loop to get struct data
		while(fgets(buff[i], STRSIZE, fp) != NULL){
			i++;
		}
		
		//Close the FILE pointer
		fclose(fp);

		//Test that info was full
		if(i<STCTSIZE){
			fp = fopen(filen, "w");
			i = 0;
			while(i <= STCTSIZE){
				fputs("0\n", fp);
				i++;
			}
			fclose(fp);
		}
						
	
		//Put info into struct;
		np->debit_bal = atof(buff[0]);
		np->credit_bal = atof(buff[1]);
		np->saving = atof(buff[2]);
		
		return np;
	}

	//Modify struct values with user input
	double modify(double val){
		double operand;
		char input[STRSIZE];

		printf("Enter number to add, precede by '-' to subtract\n%.2f\n", val);
		
		flush();
		fgets(input, STRSIZE, stdin);
		operand = atof(input);

		val += operand;
		
		return val;
	}


	//Display struct info
	void display(struct usr *np){
		int c;
		double tempdeb = np->debit_bal;
		double tempcred = np->credit_bal;
		double tempsav = np->saving;

		while(c != '0'){
			
			printf("0. Exit\n1. Debit: %.2f\n2. Credit: %.2f\n3. Savings: %.2f\n", np->debit_bal, np->credit_bal, np->saving);
			c = getchar();

			switch(c){
			
				case '0':
					break;

				case '1':
					np->debit_bal = modify(tempdeb);
					tempdeb = np->debit_bal;
					break;

				case '2':
					np->credit_bal = modify(tempcred);
					tempcred = np->credit_bal;
					break;

				case '3':
					np->saving = modify(tempsav);
					tempsav = np->saving;
					break;

				default :
					break;
			}
		}


	}	

	//Update file with new struct values
	void update(struct usr *np){
		char buff[STCTSIZE][MAXD];
		int i = 0;

		my_ftoa(np->debit_bal, buff[0]);
		my_ftoa(np->credit_bal, buff[1]);
		my_ftoa(np->saving, buff[2]);		
		
		free(np);
		
		FILE *fp;
		fp = fopen(FILENAME, "w");
		if(fp == NULL){
			fputs("Bad pointer\n", stderr);
			exit(0);
		}
		while(i<=STCTSIZE){
		//	fputs(buff[i], fp);
			fprintf(fp, "%s\n", buff[i]);
			printf("%s\n%d\n", buff[i], i);

			i++;
		}

		fclose(fp);
		
	}
	
	int main(){
		struct usr *np;
		struct usr *fileget();		

		//Get struct vals	
		np = fileget();
			
		display(np);		

		update(np);

		return 0;
	}
