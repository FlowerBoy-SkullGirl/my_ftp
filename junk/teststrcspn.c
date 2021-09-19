#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	char yes[300] = "true";
	char no[300] = "false";
	int which;
	int test;
	char maybe[2][300] = {"true", "false"};
	char * nthisis;

	test = strcspn(yes, maybe[0]);
	printf("%d\n", test);

	nthisis = strpbrk(yes, maybe[1]);
	test = strcmp(yes, nthisis);

	if(test != 0){
	printf("%s\n", nthisis);
              } else{
              	nthisis = "false";
              	printf("%s\n", nthisis);
              }

              printf("nthisis: %s, test: %d");
	return 0;
}