#include <stdio.h>
#include <stdlib.h>

#define MAXDIS 5

int main(){


	int graph[MAXDIS][MAXDIS]={0};

	graph[0][3] = 5;
	graph[4][3] = 4;

/*	for(int i=0; i<MAXDIS; i++){
		for(int j=0; j<MAXDIS; j++){
			printf("Graph x: %d y: %d value is: %d\n", i, j, graph[i][j]);
		}
	} */

	int i = 0;
	int j = 0;
	char c;

	scanf("\n%c", &c);

	while(c != 'q'){
		switch(c){
			case 'w':
			if(j<4){
				j++;
			}
			break;
			case 's':
			if(j>0){
				j--;
			}
			break;
			case 'a':
			if(i>0){
				i--;
			}
			break;
			case 'd':
			if(i<4){
				i++;
			}
			break;
		}
		printf("Location: x: %d y: %d\n", i, j);
		if(graph[i][j]>0){
			printf("You found treasure here!\n");
		}
		scanf("\n%c", &c);
	}


	return 0;
}
