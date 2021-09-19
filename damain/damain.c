#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dameanwrites.c"
#include "dameanreadup.c"

void write(void);
void read(void);


int main(){

	int done = 0;
	char option[300];
	
	printf("Welcome!\n");
	while(done == 0){

	printf("write, read, or done:\n");
	scanf("%s", option);

		if(strcmp(option, "write") == 0){
			write();
				}else if(strcmp(option, "read") == 0){
					read();
			}else if(strcmp(option, "done") == 0){
				done = 1;
			} else{	
			printf("Please try again\n");
			}

          }


}