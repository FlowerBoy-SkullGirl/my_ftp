#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>


#define HEIGHT 10 //size of window
#define WIDTH 30 // size of window
#define num_choices 4 //number of menu choices
#define MAX_SIZE_STRING 32

	void print_menu(WINDOW *menu_win, int highlight); //displays menu with proper highlight

	int y = 0; 
	int x = 0;
	char *menu[num_choices] = { //menu choices
		"1. Debit",
		"2. Credit",
		"3. Blank",
		"4. Exit",
	};

	int main(){
		
		WINDOW *menu_win;
		WINDOW *pickwin1;
		int highlight = 1;
		int choice = 0;
		
		double debitcount = 0.0;
		char *input;
		input = malloc(MAX_SIZE_STRING);

		x = (80 - WIDTH) / 2;
		y = (24 - HEIGHT) / 2;
		int ch;

		//Make curses screen
		initscr();
		noecho(); //do not show user input
		cbreak(); //take raw input
	
		clear();	
		keypad(menu_win, TRUE); //allow arrow keys
		
		menu_win = newwin(HEIGHT, WIDTH, y, x);
		
	while(choice != num_choices){ //while user does not exit
		choice = 0;
		print_menu(menu_win, highlight);
		//Loop for menu moving attr A_REVERSE
		while(ch){
			ch = wgetch(menu_win);
			switch(ch){
				case 65: //key_up
					if(highlight == 1){
						highlight = num_choices; //loop menu to bottom
					}
					else{
						--highlight;
					}
					break;
				case 66: //key_down
					if(highlight == num_choices){
						highlight = 1; //loop menu back to top
					}
					else{
						++highlight;
					}
					break;
				case 10: //user hits enter
					choice = highlight;
					break;

				default:
					break;
			}
			if(choice != 0){ //choice has been made
				break;
			}
			print_menu(menu_win, highlight);//display updated information
				
		}
		
		switch(choice){
			case 1:  //Asks user input for boys as str, atoi
				clear();
				echo();//show user what they are typing
				mvprintw(y, x, "Enter amount to add or subtract from account: ");
				getstr(input);	
				debitcount += atof(input);
				
				//Prints new screen with number of boys
				noecho();//no longer want user to see input
				wrefresh(menu_win);
				clear();
				mvprintw(y, x, "Balance: %f", debitcount);
				wrefresh(menu_win);
				getch();

				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
		}
	
	}
		endwin(); //distroy window
		return 0;
	}


	void print_menu(WINDOW *menu_win, int highlight){

		int a, b, c;

		a = 2;
		b = 2;
		
		//Makes a box around the menu
		box(menu_win, 0, 0);
		for(c = 0; c< num_choices; c++){ //Loop through the number of choices
			if(highlight == c + 1){ //Highlight the choice
				wattron(menu_win, A_REVERSE);
				mvwprintw(menu_win, b, a, "%s", menu[c]);
				wattroff(menu_win, A_REVERSE);
			}
			else{ //Print all the other choices normally
				mvwprintw(menu_win, b, a, "%s", menu[c]);
			}
			++b; //Increment y level to print on
		}
		wrefresh(menu_win); //display new info
	}
