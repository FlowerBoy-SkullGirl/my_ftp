#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct damness{

	int bigness;
	char catch[100];
};

struct textlines{

	char text[2000];

};

int main(){
	
	int der[3][2] = {
					{1, 2},
					{2, 3},
					{3, 4}
		};

		int x, y;
		for(x = 0; x < 3; x++){
			for(y = 0; y < 2; y++){
				printf("%d  ", der[x][y]);
			}
			printf("\n");
		}

	char* damn = "Damn0";
	printf("%s\n", damn);

	struct damness dood;
	dood.bigness;
	dood.catch;
	printf("What is the bigness of dude?\n");
	scanf("%d", &dood.bigness);
	printf("What it dood?\n");
	scanf("%s", dood.catch);
	printf("Dood's bigness: %d\nDood's catchphrase:%s\n", dood.bigness, dood.catch);

	printf("%p\n", &dood.bigness);


	FILE * fpointer = fopen("secondc.txt", "w");
	fprintf(fpointer, "Welcome!\nDood's bigness: %d\nDood's catchphrase:%s\n", dood.bigness, dood.catch);

	fclose(fpointer);

	struct textlines textr;
	textr.text[2000];

	struct textlines texta;
	texta.text[2000];

	struct textlines textb;
	textb.text[2000];

	struct textlines textc;
	textc.text[2000];

	struct textlines textd;
	textd.text[2000];

	FILE * fpointertwo = fopen("secondc.txt", "r");

	fgets(textr.text, 2000, fpointertwo);
	printf("%s", textr.text);
		fgets(texta.text, 2000, fpointertwo);
	printf("%s", texta.text);
		fgets(textb.text, 2000, fpointertwo);
	printf("%s", textb.text);
		fgets(textc.text, 2000, fpointertwo);
		if(textc.text == textb.text){
	printf("%s", textc.text);
		fgets(textd.text, 2000, fpointertwo);
	printf("%s", textd.text);
		} else{
			printf("Done!\n");
		}
	fclose(fpointertwo);

	

	return 0;
}