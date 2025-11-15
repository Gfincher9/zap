#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
struct stat st;
long fileLen;
int filePos=0;
int getData(char *fileLoc, int numChars, int s) {
    char data[numChars];
    
    FILE *fp = fopen(fileLoc, "r");
    if (fp == NULL) {
        perror("Error");
        return 1;
    }
    stat(fileLoc, &st);
    fileLen = st.st_size;
    while (filePos < fileLen) {
    if (numChars >= fileLen) {
        fread(data, sizeof(char), fileLen, fp);
        send(s,data,sizeof(data),0);
        fclose(fp);
        return 67;
    } 
    fseek(fp, filePos, SEEK_SET);
    if (((fileLen-1) - filePos) < numChars) {
        fread(data, sizeof(char), (fileLen-filePos),fp);
        printf("%s\n", data);
        send(s, data, sizeof(data), 0);
        fclose(fp);
        return 0;
    }
    fread(data, sizeof(char), numChars, fp);
    printf("%s\n", data); //printf will be changed to send() later
    send(s, data, sizeof(data), 0);
    filePos += numChars;
    memset(&data[0], 0, sizeof(data)); //clear out array or we'll get hold overs
    }
    fclose(fp);
    return 0;
}