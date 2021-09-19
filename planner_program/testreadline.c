#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){


	char list[300]="list.txt";

	FILE * fv;
	fv = fopen(list, "r");

	char line[300][300];
	int helper=0;	
	int user;
	int cont=1;

	printf("Reading lines, press 1 to continue, 2 to edit, 3 to exit to main\n");

	while((fgets(line[helper], 300, fv) != NULL) && (helper<300) &&(user!=3)){
		printf("%s\n", line[helper++]);

		scanf("%d", &user);


	}

	printf("Did that work?");
	fclose(fv);

	return 0;
}