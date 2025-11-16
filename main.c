#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "getData.h"
#include <stdint.h>
char filePath[4096] = "/home/gantt/programs/zap/README.md"; //this variable is where the data will be downloaded to.
char hostip[] = "0.0.0.0";

	int server(char filePath[]) {
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
			perror("You may have attempted to host too many times in the past few minutes -- ");
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
			int fileBuf;
			int i =0;
			char *path = getenv("HOME");
			strcat(path, "/.config/zap/zap.conf");
			FILE *fp = fopen(path, "r");
		if (fp == NULL) {
			perror("fopen: ");
			return 1;
	}
	fseek(fp, 8, SEEK_SET);
	while ((fileBuf = fgetc(fp)) != EOF && fileBuf != ',') {
			hostip[i] = (char)fileBuf;
			i++;
		}
		fclose(fp);
			 //change based on what the host ip is - will be in config file that gets generated or altered with flags in program
			char buf[8192];
			buf[0] = 'a'; //initalize buffer so it isnt the same as lastmsg - without this line 52-56 dont exec
			char filePath[4096];
			char lastmsg[8192];
			struct sockaddr_in address;
			int cs = socket(AF_INET, SOCK_STREAM, 0);
			if (cs<0) {
				return 2;
			}
			setsockopt(cs, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 0, sizeof(0));
			address.sin_family = AF_INET;
			address.sin_port = htons(8888); //will also be determined in config. const for now
			inet_pton(AF_INET, hostip, &address.sin_addr); 
			if (connect(cs, (struct sockaddr *)&address, sizeof(address)) < 0) {
				return 3;
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

int setConfig(char ip[]) {
char *path = getenv("HOME");
strcat(path, "/.config/zap/zap.conf");
FILE *fp = fopen(path, "w");
if (fp == NULL) {
	perror("fopen: ");
	return 1;
}
fseek(fp, 8, SEEK_SET);
fprintf(fp, "host_ip: %s,", ip);
fclose(fp);
return 0;
}

int main(int argc, char *argv[]) {
	if (argv[1] == NULL) {
	       printf("Error: invalid choice of host or client. Use this syntax: $ zap [host,client,config]\n");	
	}	
	else if (strcmp(argv[1],"host") == 0) {
		printf("Hosting\n");
		return server(filePath);
	}
	else if (strcmp(argv[1], "client") == 0) {
		printf("Waiting on host...\n");
		return client(hostip);
	}
	else if (strcmp(argv[1], "config") == 0) {
		char ip[16];
		printf("IP of host machine: ");
		scanf("%s", ip);
		return setConfig(ip);
	}
	else {
		printf("Unknown argument\n");
	}
}

