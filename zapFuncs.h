#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
int getData(char fileLoc[], int numChars, int s) {
    //Quickly Sanitize Data
    char newFile[4096];
    if (fileLoc[0] != '~') {
        memcpy(newFile, fileLoc, strlen(fileLoc)+1);
    }
    else {
    char *homepath = getenv("HOME"); //11 chars
    memcpy(newFile+strlen(homepath)-1, fileLoc, strlen(fileLoc)+1);
    memcpy(newFile, homepath, strlen(homepath));
    }
    struct stat st;
    long fileLen;
    int filePos=0;
    char data[numChars];
    FILE *fp = fopen(newFile, "r");
    if (fp == NULL) {
        perror("Error");
        return 1;
    }
    stat(newFile, &st);
    fileLen = st.st_size;
    while (filePos < fileLen) {
    if (numChars >= fileLen) {
        fread(data, sizeof(char), fileLen, fp);
        send(s,data,sizeof(data),0);
        fclose(fp);
        return 0;
    } 
    fseek(fp, filePos, SEEK_SET);
    if (((fileLen-1) - filePos) < numChars) {
        fread(data, sizeof(char), (fileLen-filePos),fp);
        send(s, data, sizeof(data), 0);
        fclose(fp);
        return 0;
    }
    fread(data, sizeof(char), numChars, fp);
    send(s, data, sizeof(data), 0);
    filePos += numChars;
    memset(&data[0], 0, sizeof(data)); //clear out array or we'll get hold overs
    }
    fclose(fp);
    return 0;
}