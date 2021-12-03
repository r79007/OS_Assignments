#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>



int main(int argc, char* argv[]) {
    char arr[50];
    int fd=open("fifo1",O_RDONLY);
/*
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

*/

    // if(read(fd,&arr,50*8)==-1){
    //         perror("read");
    //         exit(errno);
    //     }

    for (int i = 0; i < 50; i++) {
        if (read(fd, &arr[i], 8) == -1) {
            perror("read");
            exit(errno);
        }
        printf("Received %s\n", arr[i]);
    }

    //printf("%s\n",arr);

    close(fd);

    return 0;
}