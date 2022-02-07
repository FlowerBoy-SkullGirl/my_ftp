#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


int main(){

	int row, col;
	int x= 0;
	int y = 0;
	char control;

	initscr(); //starts curses
	
	getmaxyx(stdscr, row, col); //gets the max number of rows and columns on window

	cbreak(); //allows raw user input
	noecho(); //don't show typed char on screen
	keypad(stdscr, TRUE); //allows arrow keys and function keys
	
	/*
	while(control != 'q'){
		clear(); //clear screen
		switch(control){
			case 'w':
				if(y > 0)
					y--;
				break;
			case 's':
				if(y < row-1)
					y++;
				break;
			case 'a':
				if(x > 0)
					x--;
				break;
			case 'd':
				if(x < col-1)
					x++;
				break;
			default:
				break;
		}

		move(y, x);
		*/

		attrset(A_STANDOUT); //sets a curses attribute
		printw("/o<O\\"); //print to curses buffer

		
		
	//	mvprintw(4,4, "Goodbye world"); //moves y, x, then printw
		/* can also use
		move();
		mvaddch();
		*/
		
		refresh();  //refresh screen with curses buffer
		getch();
		//control = getch();
//	}	
	endwin(); //ends curses

	return 0;
}
