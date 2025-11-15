#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
struct stat st;
long fileLen;
int filePos=0;
int getData(char *fileLoc, int numChars) {
    char data[numChars];
    
    FILE *fp = fopen(fileLoc, "r");
    if (fp == NULL) {
        perror("Error");
        return 1;
    }
    stat(fileLoc, &st);
    fileLen = st.st_size;

    while (filePos < fileLen) {
    fseek(fp, filePos, SEEK_SET);
    if (((fileLen-1) - filePos) < numChars) {
        fread(data, sizeof(char), (fileLen-filePos)+1,fp);
        printf("%s", data);
        fclose(fp);
        return 0;
    }
    fread(data, sizeof(char), numChars, fp);
    printf("%s", data); //printf will be changed to send() later
    filePos += numChars;
    memset(&data[0], 0, sizeof(data)); //clear out array or we'll get hold overs
    }
    fclose(fp);
    return 0;
}