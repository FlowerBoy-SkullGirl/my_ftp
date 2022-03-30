/*
 * Basic C Display Manager
 * A simplified display manager using curses to call bash scripts
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

//CONSTANTS
#define MAX_STR 256
#define USR_FILE "/etc/passwd"
#define USR_RANGE_MIN 999
#define USR_RANGE_MAX 6000

//CURSES
#define HEIGHT 10
#define WIDTH 30

struct usrList{
	struct usrList *next;
	char *uname;
};

//Argument 1 is the head of the list. Argument 2 will be the uname of the List node
struct usrList *push_usrList(struct usrList *head, char *src)
{
	if (head == NULL){
		head = malloc(sizeof(struct usrList));
		head->next = NULL;
		head->uname = NULL;
	}
	if (head->next != NULL){
		head->next = push_usrList(head->next, src);
	}else{
		struct usrList *np = malloc(sizeof(struct usrList));
		if (np == NULL){
			fprintf(stderr, "No malloc usrList push");
			exit(0);
		}
		np->next = NULL;
		np->uname = malloc(strlen(src)+1);
		if (np->uname == NULL){
			fprintf(stderr, "No malloc usrname push");
			exit(0);
		}
		strcpy(np->uname, src);

		if (head->uname == NULL){
			free(head);
			head = NULL;
			head = np;
		}else{
			head->next = np;
		}
	}
	return head;
}

//Function will pop entire list and deallocate memory along the way
void pop_usrList(struct usrList *head)
{
	if (head == NULL){
		fprintf(stderr, "NULL list");
		exit(0);
	}
	
	if (head->next != NULL)
		pop_usrList(head->next);
	if (head != NULL){
		free(head->uname);
		free(head);
	}
}

/*
 *Returns 1 upon finding username, 0 upon finding a system user. 
 *Takes one line of MAX_STR length as argument
 *Format is uname:passwd:UID:etc:etc:irrelevant\0
 *If line contains user, changes src to username.
 */
int parse_usr(char *src)
{
	int i = 0;
	int j = 0;
	int k = 0;
	char temp[MAX_STR]; //holds uname
	char temp_uid[MAX_STR]; //holds UID

	while (*(src+k) != '\0' && i < 3){
		if (*(src+k) == ':')
			i++;
		else if (i == 0)
			temp[j++] = *(src+k);
		else if (i == 2)
			temp_uid[j++] = *(src+k);
		
		if (i == 1 && *(src+k) == ':'){
			temp[j] = '\0';
			j = 0;
		}else if (i == 3){
			temp_uid[j] = '\0';
		}
		k++;
	}

	//USR_RANGE determines which UIDs containt human users, then we copy that uname to src
	if (atoi(temp_uid) > USR_RANGE_MIN && atoi(temp_uid) < USR_RANGE_MAX){
		strcpy(src, temp);
		return 1;
	}

	return 0;
}
		
/*
 *Use the /etc/passwd file to determine users for list, return usrList pointer to head of list
 *File path will be #define macro in case differences between systems
 */
struct usrList *fetch_usrs()
{
	struct usrList *np = NULL;
	FILE *fp;
	char buf[MAX_STR];

	fp = fopen(USR_FILE, "r");
	if (fp == NULL){
		fprintf(stderr, "Could not open file\n");
		exit(0);
	}

	while (fgets(buf, MAX_STR, fp)){
		if (parse_usr(buf)){
			np = push_usrList(np, buf);	
		}
	}
	
	fclose(fp);

	return np;
}

//Will be needed for curses display
int count_list(struct usrList *np)
{
	int i = 1;
	if (np == NULL)
		return 0;

	for (; np->next != NULL; i++)
		np = np->next;
	return i;
}

//Replace this with curses after testing
void display_usrList(struct usrList *head, WINDOW *win, int highlight, int count)
{
	int a = 2;
	int b = 2;
	char *menu[count];
	
	if (head == NULL){
		fprintf(stderr, "NULL HEAD");
		exit(0);
	}
	
	struct usrList *np = head;
	for (int i = 0; i < count; i++){
		menu[i] = np->uname;
		np = np->next;
	}


	box(win, 0, 0);
	for(int c = 0; c < count; c++){
		if (highlight == c + 1){
			wattron(win, A_REVERSE);
			mvwprintw(win, b, a, "%s", menu[c]);
			wattroff(win, A_REVERSE);
		}else{
			mvwprintw(win, b, a, "%s", menu[c]);
		}
		b++;
	}
	wrefresh(win);
}

int menu_usrList(struct usrList *np)
{
	WINDOW *usrs;
	int highlight = 1;
	int choice = 0;
	int usrcount = count_list(np);
	int x = (80 - WIDTH) / 2;
	int y = (24 - HEIGHT) / 2;
	int ch;
	
	initscr();
	noecho();
	cbreak();
	
	clear();
	keypad(usrs, TRUE);
	usrs = newwin(HEIGHT, WIDTH, y, x);
	while (choice == 0){
		display_usrList(np, usrs, highlight, usrcount);
		while(ch = wgetch(usrs)){
			switch(ch){
			case 65: //key_up
				if(highlight == 1){
					highlight = usrcount; //loop menu to bottom
				}
				else{
					--highlight;
				}
				break;
			case 66: //key_down
				if(highlight == usrcount){
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
			display_usrList(np, usrs, highlight, usrcount);
		}
	}
	endwin();
	return choice;
}

int main()
{
	struct usrList *np = fetch_usrs();
	
	menu_usrList(np);

	//Essential to deallocate used memory!!
	pop_usrList(np);

	return 0;
}
