#include <stdio.h>

int stat(){

	static int counter=0;
	counter++;
	return counter;


}

int main(){

	printf("%d\n", stat());
	printf("%d\n", stat());


	return 0;
}