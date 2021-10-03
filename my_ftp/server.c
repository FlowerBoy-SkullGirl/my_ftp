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

	//Return 0 on successful handshake
	int handshake_server(int s, char *getter){
		char initsess[MAXLEN] = "Hello, server";
		int goodhello = 1;
		//Gets message from client
		recv(s, getter, MAXLEN, 0);
		printf("Message from client: %s\n", getter);
		getter[strcspn(getter, "\n")] = 0;
		
		//If message is correct, send an int to confirm server is ready for more data
		if(strcmp(getter, initsess)){
			fprintf(stderr, "Did not receive correct signal from client");
			return 1;
		}else{
			puts("Hello received...replying");
			//Make sure int is in n order
			goodhello = htonl(goodhello);
			send(s, &goodhello, sizeof(goodhello), 0);
		}
		return 0;
	}
	
	//Returns 0 on success, sends confirmation to client before more data is received
	int getfilen(int s, char *filen){
		int gotname = 1;

		//Receive a filename from the client	
		recv(s, filen, MAXLEN, 0);
		printf("Received filename from client: %s\n", filen);
		
		//Confirm filename was received
		gotname = htonl(gotname);
		send(s, &gotname, sizeof(gotname), 0);
		
		return 0;
	}
		
	int getfileline(int s, char *getter){
		int gotline = 1;
		char endfile[MAXLEN] = "Goodbye, server";
		char gotend[MAXLEN] = "Goodbye, client\n";
		
		//Receive a line from the client
		recv(s, getter, MAXLEN, 0);
		
		//Compare line received to end message
		char tempcomp[MAXLEN];
		strcpy(tempcomp, getter);
		tempcomp[strcspn(tempcomp, "\n")] = 0;
			
		if(!strcmp(endfile, tempcomp)){
			puts("Client marked end of file...");
			//Send client confirmation of end
			send(s, gotend, strlen(gotend), 0);
			return 1;
		}else{
			gotline = htonl(gotline);
			send(s, &gotline, sizeof(gotline), 0);
			return 0; 		
		}
	}
		

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
			
			FILE *fp;
			char filen[MAXLEN];

			int status = listen(sockid, WAITERS);
			for( ; ;){
				socklen_t clilen = sizeof(addrcli);		
				s = accept(sockid, (struct sockaddr *) &addrcli, &clilen);
				puts("Connection established..");
				
				//Get init signal
				if(!handshake_server(s, getter)){
					//Receive filen
					getfilen(s, filen);
					printf("Writing file %s\n", filen);

					//Open file
					fp = fopen(filen, "w");
					if(fp == NULL){
						fprintf(stderr, "Could not write file");
						exit(1);
					}
					//Begin receiving lines to write
					while(!getfileline(s, getter)){
						fprintf(fp, "%s", getter);
						}
					}
					fclose(fp);
				}
			}

			int statusc = close(sockid);
			close(s);
	
		return 0;
	}
