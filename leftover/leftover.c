#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


/* time_t t = time(NULL);
		struct tm tm = *localtime(&t);

//this is a test remove this later
		printf("Date is: %d/%d/%d \n", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
*/

void flush(void){
	//hopefully flush the newline character
int c;
while ((c = getchar()) != '\n' && c != EOF);

}



int main(){


//Ensure that File exists without overwriting


FILE * ope = fopen("leftovers.txt", "a"); 
fclose(ope);


//Using x as var array size, since we don't know the size of the menu yet
int x;
int i=0;
char menu_name[x][255];
int menu_item;
int exit=0;


//Menu loop
while(exit!=1){


// Find the size of leftovers.txt using loop
ope = fopen("leftovers.txt", "a+");

//Check that fopen won't break
if( ope != NULL ){

//loop to get menu names and size of menu all in one convenient step POGGERS
while(fgets(menu_name[i], 255, ope)!= NULL){
i++;
}
x=i;
i=0;

//Error handling
}else{
	puts("Error reading file! Please restart"); 
}


//Make sure to close the fopen
fclose(ope);

//MAIN MENU AREA
//
//

//print out main menu with listed menu items
puts("Press num buttons to select, edit, or delete. Press q to exit.\n0. Main Menu");

//loops through size of menu to print menu items
for(int i=0; i<x; i++){
printf("%d. %s", i+1, menu_name[i]);
}
//put additional text here to add item if x<9
if(x<9){
printf("%d. Add item\n", x+1);
}


//take user input
int key;
key = getchar();

//hopefully get rid of newline without any side effects
flush();

//Test input to see if it falls within parameters AND exits if input is q
if( key>47 && key<(50+x) &&key<58 || key=='q' ){
	if(key=='q'){
		exit=1;}else{
		menu_item=key-48;}
	
}

//send to main menu if input is bad
else{
	puts("Invalid menu input");
	menu_item=0;
}

//ADD ITEM SECTION
//
//

//Checks if menu item is add item
if(menu_item>0 && menu_item==x+1 ){
puts("Enter the name of the item you would like to add:");

//get new item name and append
fgets(menu_name[menu_item], 255, stdin);
//remove \n DON"T REMOVE THIS, IT DOES NOTHING, BUT CODE BREAKS WITHOUT IT 
//I think because strcspn is telling the length of menu_name, even though I don't know how that works?
int ln = strcspn(menu_name[menu_item], "\n");


FILE * add = fopen("leftovers.txt", "a");
fprintf(add, "%s", menu_name[menu_item]);

fclose(add);

//MENU SECTION
//
//

}else if(menu_item>0 && menu_item!=x+1){
printf("%sAdded on: %s\n", menu_name[menu_item], )
}

//RETURN TO LOOP
//
//
}//this is my while menu loop

	return 0;
}


