#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/types.h>
#include<fcntl.h>

int main(int argc, char *argv){
    char* arr[50];
    int fd=open("fifo2",O_RDONLY);

    for(int i=0; i<50; i++){
        if(read(fd,&arr[i],6)==-1){
            perror("read");
            exit(errno);
        }
        char* string=arr[i];
        char* str[4];
        for(int j=0;j<4;j++){
            str[j]=string[j];
        }
        printf("recieved id: %d : %s\n",atoi(string[strlen(string)-1]),str);
    }

    close(fd);

    return 0;
}