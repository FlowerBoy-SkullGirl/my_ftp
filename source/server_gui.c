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
#include <unistd.h>
#include <pthread.h>
//#include "server.c"

#define HEIGHT 10
#define WIDTH 10
#define MAXLEN_GUI 256
#define START_OPTION 0
#define PORT_OPTION 1
#define CONNECTIONS_OPTION 2
#define EXIT_OPTION 3

void *init_server(void *args)
{
	struct ftp_sock sock_args = *((struct ftp_sock *) args);
	char port_str[MAXLEN_GUI];
	snprintf(port_str, MAXLEN_GUI, "%d", sock_args.port);
	char connections_str[MAXLEN_GUI];
	snprintf(connections_str, MAXLEN_GUI, "%d", sock_args.max_pending_connections);
	char *argse[] = {"./serv","-p",port_str,"-c",connections_str};
	execv(argse[0],argse);
}

void build_menu(WINDOW *menu_window, int highlighted_row, char **menu_strings, int num_menu_options, int y, int x, struct ftp_sock sock_args)
{
	for (int i = 0; i <= num_menu_options; i++){
		if (i == highlighted_row)
			wattron(menu_window, A_REVERSE);

		switch (i){
			case PORT_OPTION:
				mvwprintw(menu_window, y + i, x, menu_strings[i], sock_args.port);
				break;
			case CONNECTIONS_OPTION:
				mvwprintw(menu_window, y + i, x, menu_strings[i], sock_args.max_pending_connections);
				break;
			default:
				mvwprintw(menu_window, y + i, x, menu_strings[i]);
				break;
		}
		if (i == highlighted_row)
			wattroff(menu_window, A_REVERSE);
	}

}

void get_port_window()
{

}

void get_connections_window()
{

}

int main()
{

	int row, col;
	int x = 0;
	int y = 0;
	struct ftp_sock sock_args;
	sock_args.port = 4414;
	sock_args.max_pending_connections = 5;

	pthread_t server_thread;

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
	char *menu_strings[MAXLEN_GUI] = {
		"Start Server",
		"Port Number(%d)",
		"Max Connections(%d)",
		"Exit"
	};
	while (!exit){
		build_menu(start_menu, highlighted_row, menu_strings, num_menu_options, HEIGHT, WIDTH, sock_args);
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
			case '\n':
				switch (highlighted_row){
					case START_OPTION:
						pthread_create(&server_thread, NULL, init_server, (void *)&sock_args);
						break;
					case PORT_OPTION:
						get_port_window();
						break;
					case CONNECTIONS_OPTION:
						get_connections_window();
						break;
					case EXIT_OPTION:
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
