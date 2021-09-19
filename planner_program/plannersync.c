#include <stdio.h>
#include <string.h>
#include <time.h>

	void sync(void){


	char listname[300]="list.txt";
	char clist[300]="clist.txt";
	char lines[300][300];
	char clines[300][300];
	int liner;
	int cliner;
	int lineradj;
	int zero=0;
	int newday=0;
	int buffer;

	FILE * sync;
	FILE * csync;
	FILE * bsync;

	sync = fopen(listname, "r");

	if(sync != NULL){
		liner=0;
	while(fgets(lines[liner], 300, sync) != NULL){
		liner++;
	}
		lineradj=liner;


	fclose(sync);
										}else{
	printf("Sync was null\n");									} //remove this when working

	//checking lines of clist to compare
	csync = fopen(clist, "r");

	if(csync!= NULL){
		cliner=0;
	while(fgets(clines[cliner], 300, csync) != NULL && (cliner<300) ){  //SOMETHING IN PLANNERVIEW BREAKS THIS SPECIFIC LINE
		cliner++;

	}

	//printf("DEBUG: Values of liner, cliner, and lineradj: %d %d %d\n", liner, cliner, lineradj);
	fclose(csync);
	//compares date to last saved date and erases list if newday is 1
								}else{
									printf("Csync was null\n");
								} //remove when working
	//this section will compare the date in completiondate.txt to today's date

	char fulldate[300];
	char compdate[300];
	char listdate[300]="completiondate.txt";
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	snprintf( fulldate, 300, "%d%d%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900 );
	FILE * datech;

	if(datech != NULL){
		
	datech = fopen(listdate, "r");
	fgets(compdate, 300, datech);
	fclose(datech);

	newday= strcmp(compdate, fulldate);
	}
	//this section creates a new empty checklist if the date has changed or if the length has changed

	if(cliner!=liner || newday!=0){
		liner=0;
		bsync = fopen(clist, "w");
		while(liner<lineradj){
			fprintf(bsync, "%d\n", zero);
			liner++;
		}
		fclose(bsync);
	}




	printf("Lists are synced!\n");
	cliner=0;
	liner=0;
	lineradj=0;
}