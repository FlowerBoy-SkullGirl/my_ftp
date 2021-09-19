#include <stdio.h>
#include <string.h>

void passer(int x, char **sand);

int main(){

	int x=5;
	char *sand[x];

	strcpy(sand[x], "I am 5");
	strcpy(sand[0], "I am 0");
	printf("%s\n&s\n", &sand[0], &sand[x]);
	passer(x, sand);

}

void passer(int x, char **sand){
printf("%s\n&s\n", &sand[0], &sand[x]);
}