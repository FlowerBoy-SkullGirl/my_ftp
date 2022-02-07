#include <stdio.h>
#include <stdlib.h>
#include <string.h>


	int main(){
		char name[16];
		char *newname;
		name[0] = 'H';
		
		newname = name;
		
		printf("%s\n", newname);

		return 0;
	}
