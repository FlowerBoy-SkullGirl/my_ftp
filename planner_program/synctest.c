#include <stdio.h>
#include <string.h>
#include <time.h>

	int main(){


	char listname[300]="list.txt";
	char clist[300]="clist.txt";
	char lines[300][300];
	int liner;
	int cliner;
	int lineradj;
	int zero=0;

	FILE * sync;
	sync= fopen(listname, "r");

	while(fgets(lines[liner], 300, sync) != NULL){
		liner++;

	}
			printf("Number of lines: %d\n", liner);
		lineradj=liner;
	fclose(sync);
	//checking lines of clist to compare
	sync= fopen(clist, "r");

	while(fgets(lines[cliner], 300, sync) != NULL){
		cliner++;

	}
			printf("Number of lines: %d\n", cliner);


	fclose(sync);

	if(cliner!=liner){
		liner=0;
		sync = fopen(clist, "w");
		printf("Values of liner: %d and lineradj: %d\n", liner, lineradj);
		while(liner<lineradj){
			fprintf(sync, "%d\n", zero);
			liner++;
		}
		fclose(sync);
	}else{
		printf("Lists are already synced!\n");
	}
	printf("Lists are synced!\n");

 return 0;
}