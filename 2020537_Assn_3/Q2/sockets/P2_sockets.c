#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "Socket"  //


int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    char buffer[10];
    
    int data_socket;
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1)
    {
        perror("socket");
        exit(errno);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
    int ret;
    ret = connect(data_socket, (const struct sockaddr *)&addr, sizeof(struct sockaddr_un));
   
    if (ret == -1)
    {
        perror("connect");
        exit(errno);
    }
    
    int k;
    k=0;
    for(;;){
    int recieved=0;
    while(1){    
        memset(buffer, 0, 10);
        
        int nread=read(data_socket,buffer,8);
        if(nread==-1){
            perror("read");
            exit(errno);
        }else{
            k++; 
            recieved++;
        }
        char rec[10];
        strcpy(rec,buffer);
        char* p;
        p=strtok(rec," ");
        printf("String recieved is : %s and it's id is %s\n", p, buffer+5);

        

    
        recieved=0;
        char mxid[8];
        sprintf(mxid,"%s",buffer+5);
        int nwrite=write(data_socket,mxid,8);
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
    //     if(k<50){
    //     recieved=0;
    //     char mxid[8];
    //     sprintf(mxid,"%s",buffer+5);
    //     int nwrite=write(data_socket,mxid,8);
    //     if(nwrite==-1){
    //         perror("write");
    //         exit(errno);
    //     }
    // }

    break;
}

    

    close(data_socket);   
    

    return 0;
}