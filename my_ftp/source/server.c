#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define WAITERS 1
#define MAXLEN 2048
#define SIZE_CHANGE 2
#define PAYLOAD_FLAG 1
#define SIZE_FLAG 2
#define EOF_FLAG 3
#define PAYLOAD 0x10000000
#define DIFF_SIZE 0x20000000
#define END_FLAG 0x30000000
#define CORRUPTED FLAG 0xF0000000
#define REMOVE_FLAG 0x0FFFFFFF
#define PAYLOAD_SIZE 3
	
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
	int getfilen(int s, char **filen){
		int gotname = 1;
		uint32_t size_filen = 0;
		char gotsize = 1;
		//Receive size of filen
		recv(s, &size_filen, sizeof(uint32_t), 0);
		send(s, &gotsize, 1, 0);
		size_filen = ntohl(size_filen);

		char *filentemp = (char *)malloc(size_filen);
		//Receive a filename from the client	
		recv(s, filentemp, MAXLEN, 0);

		int path = 1;
		while(path){
			path = strcspn(filentemp, "/");
			if (path != strlen(filentemp)){
				memcpy(filentemp, &filentemp[path+1], (strlen(filentemp) - path));
				path = 1;
			}else{
				path = 0;
			}
		}
		*filen = filentemp;
		printf("%s %d\n", filentemp, size_filen);
		filentemp[size_filen - 1] = '\0';
		printf("Received filename from client: %s\nSize: %ld\n", *filen, size_filen);
		
		//Confirm filename was received
		gotname = htonl(gotname);
		send(s, &gotname, sizeof(gotname), 0);
		
		return 0;
	}

	int reply_success(int s)
	{
		char success = 1;
		send(s, &success, 1, 0);
		//puts("Replied with success");
		return 0;
	}


	int end_signal(int s)
	{
		unsigned char c = 0;
		recv(s, &c, 1, 0);
		printf("Client preparing data type %x\n", c);
		if (c == SIZE_CHANGE)
			reply_success(s);
		return c;
	}

	uint32_t getfile32(int s, uint32_t *c)
	{
		int response = end_signal(s);
		uint32_t size_offset = 0;
		if (!response){
			puts("End of file signal from client");
			return 0;
		}
		//This is a magic number, please address
		//supposed to be signal for "not uint32_t size"
		if (response == SIZE_CHANGE){
			recv(s, &size_offset, sizeof(uint32_t), 0);
			size_offset = ntohl(size_offset);
			puts("Recieved offset size");
			reply_success(s);

			printf("Size is %ld\n", size_offset);
			//c = (uint32_t *)malloc(size_offset); hopefully don't need this trickery

			recv(s, c, size_offset, 0);
			*c = ntohl(*c);
			puts("Recieved last data");
			reply_success(s);

			return size_offset;
		}

		recv(s, c, sizeof(uint32_t), 0);
		*c = ntohl(*c);
		//puts("Recieved data");
		//reply_success(s);
		
		return sizeof(uint32_t);
	}

	int getfilec(int s, char *c)
	{
		int response = end_signal(s);
		uint32_t size_offset = 0;
		if (!response)
			return 1;
		
		recv(s, c, sizeof(char), 0);
		reply_success(s);
		
		return 0;
	}
	
	uint32_t *decapsulate(uint32_t *data, uint32_t *flag)
	{
		*flag = 0xF0000000 & *data;
		*data = *data & REMOVE_FLAG;
		return data;
	}

	uint32_t incoming_data(int s, uint32_t *c, uint32_t *flag)
	{
		recv(s, c, sizeof(uint32_t), 0);
		*c = ntohl(*c);
		c = decapsulate(c, flag);
		if (*flag == PAYLOAD)
			return PAYLOAD_SIZE;
		else if (*flag == DIFF_SIZE)
			//Data with a size_flag is the size in bytes of the next payload
			return *c;
		else if (*flag == EOF_FLAG)
			return 0;
		return 0;
	}

	//Returns 0 on failure
	uint32_t incoming_data_last(int s, uint32_t *c, uint32_t *flag)
	{
		recv(s, c, sizeof(uint32_t), 0);
		*c = ntohl(*c);
		c = decapsulate(c, flag);
		if (*flag == PAYLOAD)
			return 1;
		else if (*flag == EOF_FLAG)
			return 0;
		return 0;
	}

	int main()
	{
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
	
		if (bind(sockid, (struct sockaddr *) &addrport, sizeof(addrport))!= -1){
			puts("Socket bound..");
			
			FILE *fp;
			char *filen;

			int status = listen(sockid, WAITERS);
			for( ; ;){
				uint32_t *c = (uint32_t *)malloc(sizeof(uint32_t));
				uint32_t *flag = (uint32_t *)malloc(sizeof(uint32_t));
				if (c == NULL)
					continue;

				socklen_t clilen = sizeof(addrcli);		
				s = accept(sockid, (struct sockaddr *) &addrcli, &clilen);
				puts("Connection established..");
				
				//Get init signal
				if(!handshake_server(s, getter)){
					//Receive filen
					getfilen(s, &filen);
					printf("Writing file %s\n", filen);

					//Open file
					fp = fopen(filen, "w");
					if(fp == NULL){
						fprintf(stderr, "Could not write file");
						exit(1);
					}

					uint32_t size_message = 1;
					*flag = 0;
					while (size_message = incoming_data(s, c, flag)){
						if (*flag == PAYLOAD){
							fwrite(c, size_message, 1, fp);
						}else if (size_message < PAYLOAD_SIZE){
							incoming_data_last(s, c, flag);
							fwrite(c, size_message, 1, fp);
						}
					}

					fclose(fp);
					puts("File write success");

					if (filen != NULL)
						free(filen);

					if (c != NULL)
						free(c);
				}
			}	
		
		int statusc = close(sockid);
		close(s);
		}
		return 0;
	}
