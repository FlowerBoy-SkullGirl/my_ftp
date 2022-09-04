//Arguments command: server-ip file-to-send
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLEN 2048
	
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
		char success = 1;
		char failure = 0;
		char recieved = 0;
	
		//Send preliminary hello
		int serverready = handshake_client(sockid, sended);

		uint32_t *c = (uint32_t *)malloc(32);
		if (c == NULL){
			puts("Could not allocate memory for filereader");
			exit(1);
		}
		//Sends contents of a file
		if(fp != NULL && !serverready){
			//truncate filename before transmission
			char *filenout = (char *)malloc(strlen(filen)+1);
			strcpy(filenout, filen);
			int path = 1;
			while(path){
				path = strcspn(filenout, "/");
				if (path != strlen(filenout)){
					memcpy(filenout, &filenout[path+1], (strlen(filenout) - path));
					path = 1;
				}else{
					path = 0;
				}
			}
			filenout[strlen(filenout)+1] = '\0';

			//send size filename
			uint32_t filensize = strlen(filenout) + 1;
			char gotfilesize = 0;
			filensize = htonl(filensize);
			send(sockid, &filensize, sizeof(uint32_t), 0);
			recv(sockid, &gotfilesize, 1, 0);

			//Send filen
			send(sockid, filenout, strlen(filenout), 0);
			printf("Sent filename: %s\n", filenout);
			recv(sockid, &gotit, MAXLEN, 0);
			//Make sure bytes arrived in order
			gotit = ntohl(gotit);	
			
			printf("Server returned: %d\n", gotit);
			if(gotit){
				puts("Server received filen");
/*
				while(fgets(buff, MAXLEN, fp)){
					gotit = 0;

					//Send length of str
					length_sent = strlen(buff);
					if (length_sent == 0)
						continue;
					length_sent = htonl(length_sent);
					send(sockid, &length_sent, sizeof(length_sent), 0);
					puts("Sent length of packet");

					//Receive confirmation
					while(!gotit){
						recv(sockid, &gotit, MAXLEN, 0);
						gotit = ntohl(gotit);
						
						printf("Server received length of %d: %d\n", ntohl(length_sent), gotit);
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
				} */
				//get size of file
				fseek(fp, 0, SEEK_END);
				uint32_t endf = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				puts("Initiating data transfer");
				for (uint32_t i = ftell(fp); i <= (endf - 32); i = ftell(fp)){
					fread(c, sizeof(uint32_t), 1, fp);
					recieved = 0;
					//prepare server for data
					send(sockid, &success, 1, 0);
					//puts("Preparing data");
/*					while (!recieved){
						recv(sockid, &recieved, 1, 0);
					//	puts("Server ready");
					}
					recieved = 0;
*/					//send data
					*c = htonl(*c);
					send(sockid, c, sizeof(uint32_t), 0);
					//puts("Sent data");
/*					while (!recieved){
						recv(sockid, &recieved, 1, 0);
					//	puts("Server recieved data");
					}
					recieved = 0;
*/				}
				uint32_t length_remaining = endf - ftell(fp);
				fread(c, length_remaining, 1, fp);
				recieved = 0;
				char new_size = 2;
				send(sockid, &new_size, 1, 0);
				puts("Alert server of size mismatch");
				while (!recieved){
					recv(sockid, &recieved, 1, 0);
					puts("Server was alerted");
				}
				recieved = 0;

				length_remaining = htonl(length_remaining);
				send(sockid, &length_remaining, sizeof(length_remaining), 0);
				puts("Sent size remaining");
				while (!recieved){
					recv(sockid, &recieved, 1, 0);
					puts("Server recieved remaining size");
				}
				recieved = 0;

				*c = htonl(*c);
				send(sockid, c, length_remaining, 0);
				puts("Sent data!");
				while (!recieved){
					recv(sockid, &recieved, 1, 0);
					puts("Server recieved last data");
				}
			} 
			fclose(fp);
		}
		if (c != NULL)
			free(c);
		//Inform server that all data is received
		send(sockid, &failure, 1, 0);
		puts("Sent end of file flag");
/*		//Send length of str
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
*/
		//close connection
		int statusc = close(sockid);

		return 0;
	}
