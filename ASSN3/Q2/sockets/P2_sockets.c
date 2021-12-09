#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"  //
#define BUFFER_SIZE 10

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[10];

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1)
    {
        perror("socket");
        exit(errno);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *)&addr, sizeof(struct sockaddr_un));
   
    if (ret == -1)
    {
        fprintf(stderr, "The server is down.\n");
        exit(errno);
    }
    
    int k;
    k=0;
    for(;;){
    int recieved=0;
    while(recieved<5 && k<50){    
        memset(buffer, 0, BUFFER_SIZE);
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

        
        // if(k==50){
        //     break;
        // }
    }
        recieved=0;
        char mxid[4];
        sprintf(mxid,"%s",buffer+5);
        int nwrite=write(data_socket,mxid,4);
        if(nwrite==-1){
            perror("write");
            exit(errno);
        }
    }

    

    close(data_socket);   
    

    return 0;
}