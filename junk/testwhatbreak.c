#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){


	char slang[200];
	int check, balance = 0;
	const char slangcheck[200] = "hello";

	while(balance == 0){
		
		printf("Give me slang:\n");

		fgets(slang, 200, stdin);
		slang[strcspn(slang, "\n")] = '\0';
		check = strcmp(slang, slangcheck);

		printf("%d\n%s\n%s\n", check, slang, slangcheck);

		if(check == 0){

			balance = 1;
		}
		else{
			balance = 0;
		}

	}

	return 0;
}