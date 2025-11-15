#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
	if (argv[1] == NULL) {
	       printf("Error: invalid choice of host or client. Use this syntax: $ zap [host,client]\n");	
	}	
	else if (strcmp(argv[1],"host") == 0) {
		printf("Hosting\n");
	}
	else if (strcmp(argv[1], "client") == 0) {
		printf("Waiting on host...\n");
	}
	else {
		printf("Unknown argument\n");
	}
}

