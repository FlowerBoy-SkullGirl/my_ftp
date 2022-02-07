#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define STRSIZE 32
#define STCTSIZE 2
#define SIGDIG 2
#define MAXD 256	
#define FILENAME "financr_info.txt"
#define HEIGHT 10
#define WIDTH 30
#define NUM_ITEMS 4

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


	//Prints a curses menu
	void print_menu(WINDOW *menu_win, int highlight, char *menu_options[NUM_ITEMS], struct usr *np){
		int x, y, i;
		
		x = 0;
		y = 0;
		
		clear();	
		box(menu_win, 0, 0);
		for(i = 0; i < NUM_ITEMS + 1; i++){
			if(highlight == i + 1){
				attron(A_REVERSE);
				mvwprintw(menu_win, y, x, "%s", menu_options[i]);
				attroff(A_REVERSE);
			}
			else{
				mvwprintw(menu_win, y, x, "%s", menu_options[i]);
			}
			if(i = 1)
				wprintw(menu_win, "%.2f", np->debit_bal);
			if(i = 2)
				wprintw(menu_win, "%.2f", np->credit_bal);
			if(i = 3)
				wprintw(menu_win, "%.2f", np->saving);
			y++;
		}
		refresh();
	}


	//Modify struct values with user input
	double modify(double val, WINDOW *menu_win){
		double operand;
		char input[STRSIZE];

		clear();
		wprintw(menu_win, "Enter number to add, precede by '-' to subtract\n%.2f\n", val);
		
		echo();
		refresh();
		flush();
		getstr(input);
		operand = atof(input);

		val += operand;
		
		noecho();
		clear();
		
		return val;
	}
	
	//Display struct info
	void display(struct usr *np){
		int c;
		int d;
		double tempdeb = np->debit_bal;
		double tempcred = np->credit_bal;
		double tempsav = np->saving;
		

		WINDOW *menu_win;
		int startx = (80 - WIDTH) / 2;
		int starty = (24 - HEIGHT) / 2;

		char *menu_choices[NUM_ITEMS] ={
			"0. Exit",
			"1. Debit: %.2f",
			"2. Credit: %.2f",
			"3. Savings: %.2f" 
		};

		menu_win = newwin(HEIGHT, WIDTH, starty, startx);
		int highlight = 1;
		while(c != '0'){
			print_menu(menu_win, highlight, menu_choices, np);
			d = getch();
			while(d != '\n'){
				switch(d){
					case 65: //key up
						if(highlight != 1)
							highlight--;
						break;
					case 66: //key down
						if(highlight != NUM_ITEMS)
							highlight++;
						break;
					case 10: //enter key
						c = highlight;
						break;
				}
			}
			switch(c){
			
				case '0':
					break;

				case '1':
					np->debit_bal = modify(tempdeb, menu_win);
					tempdeb = np->debit_bal;
					break;

				case '2':
					np->credit_bal = modify(tempcred, menu_win);
					tempcred = np->credit_bal;
					break;

				case '3':
					np->saving = modify(tempsav, menu_win);
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
		
		initscr();
		noecho();
		keypad(stdscr, TRUE);
			
		display(np);		
	
		endwin();

		update(np);

		return 0;
	}
