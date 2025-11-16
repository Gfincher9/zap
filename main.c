#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "getData.h"
#include <stdint.h>
	int server() {
		char filePath[4096] = "/home/gantt/programs/zap/README.md"; //this variable is where the data will be downloaded to.
		struct sockaddr_in address;
		socklen_t adlen = sizeof(address);
		int cs = socket(AF_INET, SOCK_STREAM, 0); //cs - connection socket
		if (cs < 0) {
			return 1; //failed to make socket
		}
		setsockopt(cs, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 0, sizeof(0));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY; //only here for testing -- eventually this will be determined by user for security purposes 
		address.sin_port = htons(8888); //will also be determined in config. const for now
		if (bind(cs, (struct sockaddr*) &address, sizeof(address)) < 0) {
			return 2; //failed bind 
		 }
		 if (listen(cs, 5) < 0) {
			return 3; //connection not established
		 }
		 int ds = accept(cs, (struct sockaddr *)&address, &adlen);
		 if (ds < 0) {
			return 4; //couldnt create datasocket
		 }
		 send(ds, filePath, 4096, 0); //send path to be downloaded - let filepath = PATH_MAX (if ur path name is higher than this something is really off)
		 getData(filePath, 8192, ds); // send data 
		 close(ds);
		 close(cs);
		 return 0;
	}
	int client() {
			char buf[8192];
			buf[0] = 'a'; //initalize buffer so it isnt the same as lastmsg - without this line 52-56 dont exec
			char filePath[4096];
			char lastmsg[8192];
			struct sockaddr_in address;
			int cs = socket(AF_INET, SOCK_STREAM, 0);
			if (cs<0) {
				return 1;
			}
			setsockopt(cs, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 0, sizeof(0));
			address.sin_family = AF_INET;
			address.sin_port = htons(8888); //will also be determined in config. const for now
			inet_pton(AF_INET, "127.0.0.1", &address.sin_addr); 
			if (connect(cs, (struct sockaddr *)&address, sizeof(address)) < 0) {
				return 2;
			}
			read(cs, filePath, sizeof(filePath));
			printf("*--FILE DATA WILL BE DOWNLOADED TO %s--*\n\n", filePath); //here for testing 
			while (strcmp(buf,lastmsg)!=0) {
			read(cs, buf, sizeof(buf));
			printf("%s\n", buf);
			memcpy(lastmsg, buf, sizeof(buf));
			}
			close(cs);
			return 0;
	}
int main(int argc, char *argv[]) {
	if (argv[1] == NULL) {
	       printf("Error: invalid choice of host or client. Use this syntax: $ zap [host,client]\n");	
	}	
	else if (strcmp(argv[1],"host") == 0) {
		printf("Hosting\n");
		return server();
	}
	else if (strcmp(argv[1], "client") == 0) {
		printf("Waiting on host...\n");
		return client();
	}
	else {
		printf("Unknown argument\n");
	}
}

