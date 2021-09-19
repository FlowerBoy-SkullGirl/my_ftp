#include <stdio.h>

void view(void){

	//this will read file and ask user if they want to edit
	char listname[300]="list.txt";


	FILE * fv;
	fv= fopen(listname, "r+");

	char line[300][300];
	int helper=0;	
	int user;
	int cont=1;
	int helperfin;
	int editcheck=0;
	char in[300];
	char newline[300][300];
	int useradd=1;
	int savechanges;
	int userexit=0;

	printf("For each line, press 1 to continue, 2 to edit, 3 to exit to main\n");

	//if lines are available and user does not exit, read and print line
	//line is now an array of lines
	userexit=0;
	helper=0;
	user=0;
	while((fgets(line[helper], 300, fv) != NULL) && (helper<300) &&(user!=3)){ //this loops BREAKS the while loop in plannersync and I don't know WHY
		printf("%s", line[helper]);
		user=0;
		scanf("%d", &user);

		//make this part work
		if(user == 2){
			editcheck=1; //checks that list has been edited

			printf("Enter replacement list item or hit only enter to delete\n");
			//this is to get the \n to not skip fgets
			fgets(in, 300, stdin);

			fgets(in, 300, stdin);
			strcpy(newline[helper], in);

			printf("Line edited\n");
		}else if(user==3){
		printf("Exiting to main...\n");
		userexit=1;
		break;
	}else{
			
				strcpy(newline[helper], line[helper]);
			  }
			helper++;
	}
	if(fgets(line[helper], 300, fv) ==NULL){
		printf("End of list!\n");
		helperfin=helper;
	}


	if(userexit!=1){
	while(useradd==1){
	printf("Would you like to add to the list? 1. Yes 2. No\n");
	scanf("%d", &useradd);
		if(useradd==1){

			editcheck=1;
			printf("Enter new line:\n");

			fgets(in, 300, stdin);

			fgets(in, 300, stdin);
			strcpy(newline[helper], in);

			printf("Line added\n");

			helper++;
		}
	
					}
					helperfin=helper;
	fclose(fv);
	//reset helper and reprint array until we reach the final line
	helper=0;
		if(editcheck==1){
		printf("New list:\n");
	for(helper=0; helper<helperfin; helper++){
		printf("%s\n", newline[helper]);

											 }
		printf("Save changes? 1. Yes 2. No\n");
		scanf("%d", &savechanges);
		if(savechanges==1){
			helper=0;
			fv=fopen(listname, "w");
			for(helper=0; helper<helperfin; helper++){
				fprintf(fv, "%s", newline[helper]);
					}
			fclose(fv);
			printf("Saving and ");
		}else{
			printf("Exiting without saving\n");
		}
					}
	printf("Returning to main menu...\n");
}else if(userexit==1){
	fclose(fv);
}
}