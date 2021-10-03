//Arguments command: server-ip file-to-send
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLEN 511
	
/*	unsigned int inet_addr(char *str){
		int a, b, c, d;
		char arr[4];
		sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
		arr[0] = a; arr[1] = b, arr[2] = c, arr[3] = d;
		return *(unsigned int *)arr;
	}
*/	
	int main(int argc, char *argv[]){
		struct sockaddr_in addrserv;
		int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		char str[MAXLEN];
		char recieveded[MAXLEN];
		char filen[MAXLEN];
		char buff[MAXLEN];

		//Get ip, assign to str
		if(argc < 1){
		puts("enter ip addr");
		scanf("%s", str);
		}else{
		strcpy(str, argv[1]);
		}
		
		FILE *fp;
		//Get file to transfer, assign filen
		if(argc == 3){
			strcpy(filen, argv[2]);
			fp = fopen(filen, "r");
			if(fp == NULL){
				fprintf(stderr, "Could not open file\n");
				exit(1);
			}
		}else if(argc > 1 && argc < 3){
			puts("enter filename");
			scanf("%s", filen);
		}

		//Sets the server address and port
		memset(&addrserv, 0, sizeof(addrserv));		
		addrserv.sin_family = AF_INET;
		addrserv.sin_port = htons(4414);
		addrserv.sin_addr.s_addr = inet_addr(str);
		
		//Try connect to server
		int status = connect(sockid, (struct sockaddr *) &addrserv, sizeof(addrserv));
		
	
		//Init and end session codes
		char sended[MAXLEN] = "Hello, server\n";
		char endsended[MAXLEN] = "Goodbye, server\n";
		char gotit[MAXLEN];	
		//Send preliminary hello
		send(sockid, sended, strlen(sended), 0);
			
		//Sends contents of a file
		if(fp != NULL){
			//Send filen
			send(sockid, filen, strlen(sended), 0);
			puts("Sent filename");
			recv(sockid, gotit, MAXLEN, 0);

//			printf("%s", gotit);
//			puts("Server received filen");
			while(fgets(buff, MAXLEN, fp)){
				//Make gotit not gotit
				strcpy(gotit, "Notgot");
				//Send nextline
				send(sockid, buff, strlen(buff), 0);
				//See if server gotit
				while(strcmp(gotit, "File received\n") != 0){
					recv(sockid, gotit, MAXLEN, 0);
				}
			}
			fclose(fp);
		}
		//Send endliminary goodbye
		send(sockid, endsended, strlen(endsended), 0);
		
		//Receive server's confirmation
		recv(sockid, recieveded, MAXLEN, 0);
		printf("%s", recieveded);

		//close connection
		int statusc = close(sockid);

		return 0;
	}
