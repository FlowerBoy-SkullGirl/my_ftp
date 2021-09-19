#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	
	//FIX
	char *my_strcpy(char *dest, char *src){
		int i = 0;
		dest = malloc(strlen(src)+1);
		if (dest == NULL){
			puts("NUll malloc");
			return(0);
		}
		while(*(src+i) != '\0'){
			*(dest+i) = *(src+i);
			i++;
		}	
		*(dest+i) = '\0';
		printf("%s\n", dest);
		return dest;
	}

	struct node{
		int count;
		char *word;
		struct node *left;
		struct node *right;
	};

	struct node *tree(struct node *np, char *wp){
		printf("Entering tree\n");
		int comp;
		if(np == NULL){
//			printf("Allocating for struct\n");
			//allocate memory to np somehow
			np = malloc(sizeof(struct node));
			if (np == NULL){
				puts("Still null");
			}
			//copy wp into np
//			printf("Copying %s into word\n", wp);
			np->word = my_strcpy(np->word, wp);
			np->count = 1;
			np->left = np->right = NULL;
		}
		else if( (strcmp(np->word, wp)==0) ){
			puts("Word occured already");
			np->count++;
		}
		else if( comp < 0 ){
			puts("New word less");
			np->left = tree(np->left, wp);
		}
		else{
			puts("New word greater");
			np->right = tree(np->right, wp);
		}
		return np;
 	}
	
	void treeprint(struct node *np){
		if(np != NULL){
			treeprint(np->left);
			printf("%d, %s\n", np->count, np->word);
			treeprint(np->right);
			free(np);
		}
	}

	int main(){
//		char *test = NULL;
		struct node *tree();
		struct node *root = NULL;
		char *words[5] = { "hello", "wowie", "hello", "dang", "hello" };
//		if(my_strcpy(test, words[0]))
//			printf("Test %s\n", test);	
		//loop to get words
		for(int i = 0; i<5; i++){
			printf("Inserting: %s\n", words[i]);
			root = tree(root, words[i]);
		}
		//print tree results
		treeprint(root);
		return 0;
	}
