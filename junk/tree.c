#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	

	char* words[255]={
	"hello",
	"what",
	"hello",
	"wow",
	"abacus",
	"crabacus",
	"hello",
	"orange"
	};
	
	struct tree{
		struct tree *lchild;
		struct tree *rchild;
		int count;
		char *word;
	}

	struct tree *start(struct tree *root, char *search){
		int condition;
		struct tnode *talloc();
		
		if(root == NULL){
			root = talloc();
			root->word = search;
			root->count = 1;
			root->lchild = root->rchild = NULL;
		}else if( (condition = strcmp(search, root->word)) == 0){
			root->count++;
		}else if( condition < 0){
			root->left = start(root->left, search);
		}else{
			root->right = start(root->right, search);
		}
		return(root);
	}


	int main(){
		
		//choose word to search and rootoint to tree

		//comrootare search to root, then correct child, create child if does not exist

		//outrootut count of words


		return 0;
	}
