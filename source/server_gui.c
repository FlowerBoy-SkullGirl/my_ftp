#include <stdio.h> 
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "networking.h"
#include "file_metadata.h"
#include "session_queue.h"
//#include "server.c"

#define HEIGHT 10
#define WIDTH 10

int main()
{

	int row, col;
	int x = 0;
	int y = 0;
	short port = 4414;
	int max_waiting = 5;

	initscr();
	refresh();
	getmaxyx(stdscr, row, col);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	WINDOW *start_menu = newwin(row, col, y, x);

	box(start_menu, 0, 0);
	int exit = 0;
	int highlighted_row = 0;
	int key_in;
	int num_menu_options = 3;
	while (!exit){
		if (highlighted_row = 0){
			wattron(start_menu, A_REVERSE);
			mvwprintw(start_menu, HEIGHT, WIDTH, "Start server");  
			wattroff(start_menu, A_REVERSE);
		}else{
			mvwprintw(start_menu, HEIGHT, WIDTH, "Start server");  
		}
		if (highlighted_row = 1){
			wattron(start_menu, A_REVERSE);
			mvwprintw(start_menu, HEIGHT + 1, WIDTH, "Port Number(%d)", port);
			wattroff(start_menu, A_REVERSE);
		}else{
			mvwprintw(start_menu, HEIGHT + 1, WIDTH, "Port Number(%d)", port);
		}
		if (highlighted_row = 2){
			wattron(start_menu, A_REVERSE);
			mvwprintw(start_menu, HEIGHT + 2, WIDTH, "Max Connections(%d)", max_waiting);
			wattroff(start_menu, A_REVERSE);
		}else{
			mvwprintw(start_menu, HEIGHT + 2, WIDTH, "Max Connections(%d)", max_waiting);
		}
		if (highlighted_row = 3){
			wattron(start_menu, A_REVERSE);
			mvwprintw(start_menu, HEIGHT + 3, WIDTH, "Exit.");
			wattroff(start_menu, A_REVERSE);
		}else{
			mvwprintw(start_menu, HEIGHT + 3, WIDTH, "Exit.");
		}

		wrefresh(start_menu);

		key_in = getch();
		switch (key_in){
			case KEY_DOWN:
				highlighted_row++;
				if (highlighted_row > num_menu_options)
					highlighted_row = 0;
				break;
			case KEY_UP:
				highlighted_row--;
				if (highlighted_row < 0)
					highlighted_row = num_menu_options;
				break;
			case '\r':
				switch (highlighted_row){
					case 3:
						exit = 1;
						break;
					default:
						break;
				}
			default:
				break;
		}
	}
	delwin(start_menu);
	endwin();

}
