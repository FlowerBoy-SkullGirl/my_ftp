#include <stdio.h>
#include <time.h>
#include <string.h>
#include "plannercheck.c"
#include "plannerview.c"
#include "plannernew.c"
#include "plannersync.c"
#include "plannerset.c"
#include "plannerrem.c"

	void check(void);
	void view(void);
	void new(void);
	void sync(void);
	void set(void);
	void rem(void);


	int main(){

		//retrieve time?
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

//this is a test remove this later
		printf("Date is: %d/%d/%d \n", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);

		printf("Hello! Let's get your lists ready!\n");

		//make sure a list exists without overwriting it
		char listname[300] = "list.txt";
		char clist[300] = "clist.txt";

		FILE * fcheck;
		fcheck = fopen(listname, "a");
		fclose(fcheck);
		fcheck = fopen(clist, "a");
		fclose(fcheck);
		
		//comparing the answer
		int response;
		int done=0;

		while(done==0){

			printf("Do you want to 1. Check list 2. View/Edit list 3. Create a list 4. Exit (Press a number key 1-4) \n");

		scanf("%d", &response);

		if(response == 1){
			sync();
			check();
			set();
		}else if(response == 2){
			view();
			rem();
		}else if(response == 3){
			new();
			rem();
		}else
			done=1;
		}	
		printf("Shutting down Listr....\n");
						}

