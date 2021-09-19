#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	char string[200]="beep boop I am a robot";
	char inject[4][200]={"helllo", "fill", "hellloo", "fill2"};
	strcpy(inject[1], string);
	printf("%s %s %s %s", inject[0], inject[1], inject[2], inject[3]);

	return 0;

}