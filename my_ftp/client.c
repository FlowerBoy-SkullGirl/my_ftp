//Arguments command: server-ip file-to-send
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLEN 2047
	
/*	unsigned int inet_addr(char *str){
		int a, b, c, d;
		char arr[4];
		sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
		arr[0] = a; arr[1] = b, arr[2] = c, arr[3] = d;
		return *(unsigned int *)arr;
	}
*/	
	//Initializes transfer of info, waits until server is ready for filename, 0 on success	
	int handshake_client(int sockid, char *sended){
		//Servers success signal is 1	
		int server_hello = 1;
		int temp = 0;
		
		//Send a hello and receive the success signal
		send(sockid, sended, strlen(sended), 0);
		recv(sockid, &temp, MAXLEN, 0);
		
		//Make sure byte order
		temp = ntohl(temp);
		
		//Verify success	
		if(temp == server_hello){
			puts("Server returned hello signal");
			return 0;
		}else{
			fprintf(stderr, "Server did not return signal");
			return 1;
		}
	}	

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
		
		printf("Connecting to %s\n", str);

		//Try connect to server
		int status = connect(sockid, (struct sockaddr *) &addrserv, sizeof(addrserv));
		
	
		//Init and end session codes
		char sended[MAXLEN] = "Hello, server\n";
		char endsended[MAXLEN] = "Goodbye, server\n";
		int gotit = 0;	
		int length_sent = 0;
	
		//Send preliminary hello
		int serverready = handshake_client(sockid, sended);

		//Sends contents of a file
		if(fp != NULL && !serverready){
			//Send filen
			send(sockid, filen, strlen(sended), 0);
			printf("Sent filename: %s\n", filen);
			recv(sockid, &gotit, MAXLEN, 0);
			//Make sure bytes arrived in order
			gotit = ntohl(gotit);	
			
			printf("Server returned: %d\n", gotit);
			if(gotit){
				puts("Server received filen");

				while(fgets(buff, MAXLEN, fp)){
					gotit = 0;

					//Send length of str
					length_sent = strlen(buff);
					length_sent = htonl(length_sent);
					send(sockid, &length_sent, sizeof(length_sent), 0);

					//Receive confirmation
					while(!gotit){
						recv(sockid, &gotit, MAXLEN, 0);
						gotit = ntohl(gotit);
						
						printf("Server received line: %d\n", gotit);
					}
					gotit = 0;
					
					//Send nextline
					send(sockid, buff, strlen(buff), 0);
					printf("Sent: %s\n", buff);
					
					//See if server gotit
					while(!gotit){
						recv(sockid, &gotit, MAXLEN, 0);
						gotit = ntohl(gotit);
						
						printf("Server received line: %d\n", gotit);
					}
				}
			}
			fclose(fp);
		}
		//Send length of str
		length_sent = strlen(endsended);
		length_sent = htonl(length_sent);
		send(sockid, &length_sent, sizeof(length_sent), 0);

		//Receive confirmation
		while(!gotit){
			recv(sockid, &gotit, MAXLEN, 0);
			gotit = ntohl(gotit);
				
			printf("Server received line: %d\n", gotit);
		}
		gotit = 0;
					

		//Send endliminary goodbye
		send(sockid, endsended, strlen(endsended), 0);
			
		printf("Sent server goodbye message\n");
		
		//Receive server's confirmation
		recv(sockid, recieveded, MAXLEN, 0);
		printf("%s\n", recieveded);

		//close connection
		int statusc = close(sockid);

		return 0;
	}
