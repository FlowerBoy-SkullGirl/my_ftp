#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define WAITERS 1
#define MAXLEN 511

	int main(){
		struct sockaddr_in addrport, addrcli;
		int sockid = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		char sender[MAXLEN] = "File received\n";
		char getter[MAXLEN];
		int s; 
		
		puts("Socket created..");
		
		addrport.sin_family = AF_INET;
		addrport.sin_port = htons(4414);
		addrport.sin_addr.s_addr = htonl(INADDR_ANY);
		
		memset(&addrcli, 0, sizeof(addrcli));
		addrcli.sin_family = AF_INET;
		addrcli.sin_port = htons(INADDR_ANY);
		addrcli.sin_addr.s_addr = htonl(INADDR_ANY);
	
		if(bind(sockid, (struct sockaddr *) &addrport, sizeof(addrport))!= -1) {
			puts("Socket bound..");
			
			char initsess[MAXLEN] = "Hello, server";
			char endsess[MAXLEN] = "Goodbye, server";
			FILE *fp;

			int status = listen(sockid, WAITERS);
			for( ; ;){
				socklen_t clilen = sizeof(addrcli);		
				s = accept(sockid, (struct sockaddr *) &addrcli, &clilen);
				puts("Connection established..");
				
				//Get init signal
				recv(s, getter, MAXLEN, 0);
				printf("%s\n", getter);
				getter[strcspn(getter, "\n")] = 0;
				send(s, sender, strlen(sender), 0);
				int hewwo = strcmp(getter, initsess);
				printf("%d\n", hewwo);
				if(!strcmp(getter, initsess)){
					printf("%s", getter);
					//Receive filen
					recv(s, getter, MAXLEN, 0);
					printf("Writing file %s\n", getter);
					send(s, sender, strlen(sender), 0);

					//Open file
					fp = fopen(getter, "w");
					if(fp == NULL){
						fprintf(stderr, "Could not write file");
						exit(1);
					}
					//Begin receiving lines to write
					while(!strcmp(getter, endsess)){
						recv(s, getter, MAXLEN, 0);
						//Make sure they are not the end signal
						if(!strcmp(getter, endsess)){
							break;
						}else{
						//Write line to file
						fprintf(fp, "%s", getter);
						}
						//Send confirmation of receipt
						send(s, sender, strlen(sender), 0);
					}
					fclose(fp);
				}
				send(s, sender, strlen(sender), 0);
			}

			int statusc = close(sockid);
			close(s);
		}
	
		return 0;
	}
