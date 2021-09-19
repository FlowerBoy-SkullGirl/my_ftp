#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HT_SIZE 100

	char *strsave(char *dest, char *src){	
		int i = 0;
		dest = malloc(strlen(src)+1);
		if (dest == NULL){
			return(0);
		}
		while(*(src+i) != '\0'){
			*(dest+i) = *(src+i);
			i++;
		}	
		*(dest+i) = '\0';
		return dest;
	}

	struct ndx{
		char *xpr;
		char *def;
		struct ndx *nxt;
	};
	
	static struct ndx *hashtab[HT_SIZE];

	int hash(char *str){
		int hashv;
		
		for(hashv = 0; *str != '\0'; )
			hashv += *str++;
		return(hashv % HT_SIZE);
	}
	
	struct ndx *search(char *str){
		struct ndx *np;

		for(np = hashtab[hash(str)]; np != NULL; np = np->nxt)
			if(strcmp(str, np->xpr) == 0){
				return(np);
			}
		return(NULL);
	}

	struct ndx *install(char *str, char *def){
		struct ndx *np, *search();
		int hashv;
		
		if((np = search(str)) == NULL){
			np = malloc(sizeof(*np));
			if(np == NULL){
				return(NULL);
			}
			if((np->xpr = strsave(np->xpr, str)) == NULL)
				return(NULL);
			hashv = hash(np->xpr);
			np->nxt = hashtab[hashv];
			hashtab[hashv] = np;
		}else
			free(np->def);
		if((np->def = strsave(np->def, def)) == NULL)
			return(NULL);
		return(np);
	}


	int main(){
		char *first = "first";
		char *sec = "second";
		char *third = "third";
		char *four = "fourth";
		char *fif = "first";
		
		char *defone = "not_first";
		char *deftwo = "not_second";
		char *defthree = "not_three";
		char *deffour = "not_four";
		char *deffif = "not_fifth";
		

		struct ndx *np;
		install(first, defone);
		np = search(first);
		printf("Def 1: %s\n", np->def);
		
		install(sec, deftwo);
		np = search(sec);
		printf("Def 2: %s\n", np->def);

		install(third, defthree);
		np = search(third);
		printf("Def 3: %s\n", np->def);

		install(four, deffour);
		np = search(four);
		printf("Def 4: %s\n", np->def);

		install(fif, deffif);
		np = search(fif);
		printf("Def 5: %s\n", np->def);

		return 0;
	}
