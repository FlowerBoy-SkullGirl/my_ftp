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

#define MAX_STR 256
#define USR_FILE "/etc/passwd"
#define USR_RANGE_MIN 999
#define USR_RANGE_MAX 6000

struct usrList{
	struct usrList *next;
	char *uname;
};

//Argument 1 is the head of the list. Argument 2 will be the uname of the List node
void push_usrList(struct usrList *head, char *src)
{
	
	if (head->next != NULL){
		push_usrList(head->next, src);
	}else{
		struct usrList *np = malloc(sizeof(struct usrList));
		if (np == NULL){
			fprintf(stderr, "No malloc usrList push");
			exit(0);
		}
		np->uname = malloc(strlen(src)+1);
		if (np->uname == NULL){
			fprintf(stderr, "No malloc usrname push");
			exit(0);
		}
		strcpy(np->uname, src);
		
		if (head->uname == NULL)
			head = np;
		else
			head->next = np;
	}

}

//Function will pop entire list and deallocate memory along the way
void pop_usrList(struct usrList *head)
{
	if (head->next != NULL)
		pop_usrList(head->next);

	free(head->uname);
	free(head);
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
	char temp[MAX_STR];
	char temp_uid[MAX_STR];

	while (*(src) != '\0' && i < 1){
		if (*(src) == ':')
			i++;
		temp[j++] = *(src++);
	}
	//No increment necessary, as this replaces the colon
	temp[j] = '\0';
	j = 0;

	while (*(src) != '\0' && i < 3){
		if (*(src) == ':')
			i++;
		temp_uid[j++] = *(src++);
	}
	temp_uid[j] = '\0';
	
	//USR_RANGE determines which UIDs containt human users, then we copy that uname to src
	if (atoi(temp_uid) > USR_RANGE_MIN && atoi(temp_uid) < USR_RANGE_MAX){
		strcpy(temp, src);
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
	struct usrList *np;
	FILE *fp;
	char buf[MAX_STR];

	fp = fopen(USR_FILE, "r");
	
	while (fgets(buf, MAX_STR, fp)){
		if (parse_usr(buf))
			push_usrList(np, buf);	
	}
	
	fclose(fp);

	return np;
}

//Will be needed for curses display
int count_list(struct usrList *np)
{
	int i = 1;
	for (; np->next != NULL; i++)
		np = np->next;
	return i;
}

//Replace this with curses after testing
void display_usrList(struct usrList *head)
{
	while (head->next != NULL){
		printf("%s\n", head->uname);
		head = head->next;
	}
}

int main()
{
	struct usrList *np = fetch_usrs();
	
	display_usrList(np);

	//Essential to deallocate used memory!!
	pop_usrList(np);

	return 0;
}
