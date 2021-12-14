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
    int data_socket;
    char buffer[10];
    char mxid[8];
    data_socket=open("fifo1",O_RDONLY);
    if(data_socket == -1){
        perror("open");
        exit(errno);
    }

    if (mkfifo("fifo2", 0666) == -1) {
        if (errno != EEXIST) {
            printf("Could not create fifo file\n");
            return 1;
        }
    }

    int data_socket2=open("fifo2",O_WRONLY);
    if(data_socket2==-1){
        perror("open");
        exit(errno);
    }
    
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
    }

*/

    // if(read(fd,&arr,50*8)==-1){
    //         perror("read");
    //         exit(errno);
    //     }

    int k;
    k=0;
    
    //for(;;){
    //int recieved=0;
    while(1){    
        memset(buffer, 0, 10);
        memset(mxid,0,8);
        int nread=read(data_socket,buffer,8);
        if(nread==-1){
            perror("read");
            exit(errno);
        }else{
            k++; 
            
            //recieved++;
        }
        char rec[10];
        strcpy(rec,buffer);
        char* p;
        p=strtok(rec," ");
        printf("String recieved is : %s and it's id is %s\n", p, buffer+5);

        // if (mkfifo("fifo2", 0777) == -1) {
        // if (errno != EEXIST) {
        //     printf("Could not create fifo file\n");
        //     return 1;
        // }
    //}
    
    
        //recieved=0;
        
        sprintf(mxid,"%s",buffer+5);
        int nwrite=write(data_socket2,mxid,8);
        if(nwrite==-1){
            perror("write");
            exit(errno);
        }

        if(atoi(buffer+5)==49){
                break;
            }

        
        // if(k==50){
        //     break;
        // }
    }
    //int data_socket2=open("fifo1",O_WRONLY);
    // if (mkfifo("fifo2", 0777) == -1) {
    //     if (errno != EEXIST) {
    //         printf("Could not create fifo file\n");
    //         return 1;
    //     }
    // }
    
    
    //     recieved=0;
    //     char mxid[8];
    //     sprintf(mxid,"%s",buffer+5);
    //     int nwrite=write(data_socket2,mxid,8);
    //     if(nwrite==-1){
    //         perror("write");
    //         exit(errno);
    //     }
    //}

    //printf("%s\n",arr);

    close(data_socket);
    close(data_socket2);

    return 0;
}