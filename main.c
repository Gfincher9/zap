#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
	int server() {
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
			return 2; //failed bind :(
		 }
		 if (listen(cs, 5) < 0) {
			return 3; //connection not established
		 }
		 int ds = accept(cs, (struct sockaddr *)&address, &adlen);
		 if (ds < 0) {
			return 4; //couldnt create datasocket
		 }
		 send(ds, "hello", strlen("hello"), 0);
		 close(ds);
		 close(cs);
		 return 0;
	}
	int client() {
			char buf[6];
			struct sockaddr_in address;
			socklen_t adlen;
			int cs = socket(AF_INET, SOCK_STREAM, 0);
			if (cs<0) {
				return 1;
			}
			setsockopt(cs, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 0, sizeof(0));
			address.sin_family = AF_INET;
			address.sin_port = htons(8888); //will also be determined in config. const for now
			if (connect(cs, (struct sockaddr *)&address, adlen) < 0) {
				return 2;
			}
			read(cs, buf, sizeof(buf));
			close(cs);
			return 0;
	}
int main(int argc, char *argv[]) {
	if (argv[1] == NULL) {
	       printf("Error: invalid choice of host or client. Use this syntax: $ zap [host,client]\n");	
	}	
	else if (strcmp(argv[1],"host") == 0) {
		printf("Hosting\n");
		server();
	}
	else if (strcmp(argv[1], "client") == 0) {
		printf("Waiting on host...\n");
		client();
	}
	else {
		printf("Unknown argument\n");
	}
}

