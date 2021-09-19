#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){

	printf("Hello!...\nConnecting to raspberry pi...\n");

	char command[300];
	char addr[300];
	strcpy(addr, "10.0.0.109");

	strcpy(command, "ssh ");
	strcat(command, addr);
	system(command);

	int connect=0;

	//while(connect==0){
	//test connection to ssh before entering passwd

	//}


	return 0;
}