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
#include <sys/types.h>
#include <pwd.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

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
	uid_t usrid;
};

static struct pam_conv conv = {
	misc_conv,
	NULL
};

struct usrList *newnode(char *src, int uid_int)
{
	struct usrList *np = malloc(sizeof(struct usrList));
	if (np == NULL){
		fprintf(stderr, "Could not malloc node\n");
		exit(0);
	}

	np->uname = malloc(strlen(src) + 1);
	if (np == NULL){
		fprintf(stderr, "Could not malloc name\n");
		exit(0);
	}
	
	strcpy(np->uname, src);
	np->usrid = uid_int;
	np->next = NULL;

	return np;
} 

//Argument 1 is the head of the list. Argument 2 will be the uname of the List node
struct usrList *push_usrList(struct usrList *head, char *src, int uid_int)
{
	if (head == NULL)
		head = newnode(src, uid_int);
	else
		head->next = push_usrList(head->next, src, uid_int);

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
 *Returns uid upon finding username, 0 upon finding a system user. 
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
		return atoi(temp_uid);
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
	int uid_int = 0;

	fp = fopen(USR_FILE, "r");
	if (fp == NULL){
		fprintf(stderr, "Could not open file\n");
		exit(0);
	}
	
	while (fgets(buf, MAX_STR, fp)){
		if (uid_int = parse_usr(buf)){
			np = push_usrList(np, buf, uid_int);	
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
	int ch = 0;
	
	initscr();
	noecho();
	cbreak();
	
	clear();
	usrs = newwin(HEIGHT, WIDTH, y, x);

	keypad(usrs, TRUE);

	while (choice == 0){
		display_usrList(np, usrs, highlight, usrcount);
		while(ch = wgetch(usrs)){
			switch(ch){
			case KEY_UP: //key_up
				if(highlight == 1){
					highlight = usrcount; //loop menu to bottom
				}
				else{
					--highlight;
				}
				break;
			case KEY_DOWN: //key_down
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

/*
 * Displays window for user to input password
 */
int display_passwd_win(pam_handle_t *pamh, struct usrList *np)
{
	char input[MAX_STR];
	WINDOW *pass;
	int highlight = 1;
	int choice = 0;
	int x = (80 - WIDTH) / 2;
	int y = (24 - HEIGHT) / 2;
	int ch = 0;
	
	initscr();
	noecho();
	cbreak();
	
	clear();
	pass = newwin(HEIGHT, WIDTH, y, x);

	keypad(pass, TRUE);

	echo();
	mvprintw(y, x, "Password:");
	if(pam_authenticate(pamh, 0) != PAM_SUCCESS){
		fprintf(stderr, "Could not auth\n");
		return 0;
	}
	noecho();
	wrefresh(pass);

	return 1;
}

/*
 * Iterates the usrlist i times to find the chosen user
 * Returns a pointer to said user struct
 */
struct usrList *get_usr_index(int index, struct usrList *np)
{
	struct usrList *choice = NULL;
	choice = np;
	for (int i = 1; i < index; i++){
		if (choice->next != NULL)
			choice = choice->next;
	}
	return choice;
}

/*
 * Returns 1 on success and 0 on failure
 * Iterates to the correct user choice, calls PAM to get the password
 */
int get_password_uid(int usrlist_index, struct usrList *head)
{
	pam_handle_t *pamh = NULL;
	int ret = 0;
	struct usrList *np = get_usr_index(usrlist_index, head);
	if (np == NULL){
		fprintf(stderr, "No user to fetch passwd\n");
		exit(0);
	}

	if (ret = pam_start("sudo", np->uname, &conv, &pamh) != PAM_SUCCESS){
		fprintf(stderr, "Could not start pam\n");
		return 0;
	}

	return display_passwd_win(pamh, np);
}
int main()
{
	struct usrList *np = fetch_usrs();
	
	int usr_choice = 0;
	int logged_in = 0;
	while (!logged_in){	
		usr_choice = menu_usrList(np);
		logged_in = get_password_uid(usr_choice, np);
	}

	//Essential to deallocate used memory!!
	pop_usrList(np);

	return 0;
}
