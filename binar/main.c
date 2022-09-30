#include <stdio.h>
#include <stdlib.h>

	int main(){
		int c = 0;

		int a = 8;
		unsigned int shiftr = 0xFF000000;
		shiftr = shiftr >> 8;
		c = c | a;
		printf("%d\n", c);
		printf("%x\n", shiftr);



		return 0;
	}
