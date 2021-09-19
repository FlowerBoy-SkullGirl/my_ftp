#include <stdio.h>
#include <stdlib.h>
#include <string.h>


	struct list{
		int val;
		struct list *next;
		struct list *prev;
	};
	
	struct list *install(int i){
		//Creats new node and allocates memory
		struct list *np;
		np = malloc(sizeof(struct list));
		if(np == NULL)
			puts("NULL node error");
		//Puts wanted value in new node and leaves an open next pointer
		np->val = i;
		np->next = np->prev = NULL;
		
		return np;
	}

	struct list *reverseb(struct list *np){
		struct list *child;
		child = np->prev;
		if(child != NULL){
			reverseb(child);
		}
		printf("%d", np->val);

	}
	
	struct list *reverse(struct list *node){
		//Takes a starting node and keeps making children until it finds the last node
		struct list *child;
		struct list *reverseb();
		child = node->next;
		if(child != NULL){
			reverse(child);
			
		}
		//Waits until at the last node then prints all in order, freeing all in order
		reverseb(node);
		printf("%d", node->val);
		free(node);
	}


	int main(){
		struct list *start, *end, *temp = NULL;
		struct list *install(), *reverse(); 
		int i = 0;
		//Starting node to keep an access point into the list
		start = install(i);
		i=1;
		//end node will help us populate the list
		end = install(i);
		//After each second node is installed, point to it as the next node of the previous
		start->next = end;
		end->prev = start;
		for(i=2; i<10; i++){
			//loop the above process
			temp = install(i);
			end->next = temp;	
			temp->prev = end;
			end = NULL;
			i++;
			end = install(i);
			temp->next = end;
			end->prev = temp;
			temp = NULL;
		}

		reverse(start);			

		return 0;
	}
