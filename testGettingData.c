#include "getData.h"
//this is just a wrapper for getData.h to test if it works
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: Not enough arguments");
        return 1;
    }
    int numChars = atoi(argv[2]);
    getData(argv[1], numChars);
}