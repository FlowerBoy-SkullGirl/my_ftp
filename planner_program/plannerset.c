#include <stdio.h>
#include <time.h>
#include <string.h>


	void set(void){


	//saves a day to feed to syncer
	char fulldate[300];
	char listdate[300]="completiondate.txt";
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	snprintf( fulldate, 300, "%d%d%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900 );
	FILE * dater;
	dater = fopen(listdate, "w");
	fprintf(dater, "%s", fulldate);
	fclose(dater);







	}