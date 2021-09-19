#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	char slang[200] = "Whattup";
	char sling[200] = "Whattup";
	int slung;

	printf("sling is: %s\n", sling);

	fgets(slang, 200, stdin);
	slang[strcspn(slang, "\n")] = '\0';
	slung = strcmp(slang, sling);
	printf("Slung is: %d\n", slung);
	printf("Slang is: %s\n", slang);
	printf("sling is: %s\n", sling);

	return 0;
}