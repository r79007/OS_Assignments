#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>

#define SOCKET_NAME "/tmp/DemoSocket"   //
#define BUFFER_SIZE 128
char data2[10];

int rdrand16_step(uint16_t *rand)
{
    unsigned char ok;

    asm volatile("rdrand %0; setc %1"
                 : "=r"(*rand), "=qm"(ok));

    return (int)ok;
}

char *gen_random(size_t length)
{ 

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // could be const
    char *randomString;

    if (length)
    {
        randomString = malloc(length + 1); // sizeof(char) == 1, cf. C99

        if (randomString)
        {
            int l = (int)(sizeof(charset) - 1); // (static/global, could be const or #define SZ, would be even better)
            int key;                            // one-time instantiation (static/global would be even better)
            for (int n = 0; n < length; n++)
            {
                uint16_t rndnum;
                rdrand16_step(&rndnum);
                key = rndnum % l; // no instantiation, just assignment, no overhead from sizeof
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}

int main(int argc, char *argv[])
{

    char* buffer2[50];    
    for(int i=0;i<50;i++){               //Storing 50 random strings of length 4
        buffer2[i] = gen_random(4);
        //printf("%s\n", buffer[i]);
    }

    struct sockaddr_un name;

    int ret;
    int connection_socket;
    int data_socket;
    int result;
    int data;
    char buffer[BUFFER_SIZE];

    unlink(SOCKET_NAME);

    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (connection_socket == -1)
    {
        perror("socket");
        exit(errno);
    }

    printf("Master socket created\n");

    memset(&name, 0, sizeof(struct sockaddr_un));
    
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *)&name, sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        perror("bind");
        exit(errno);
    }

    printf("bind() call succeed\n");

    ret = listen(connection_socket, 20);
    if (ret == -1)
    {
        perror("listen");
        exit(errno);
    }

    for (;;)
    {

        printf("Waiting on accept() sys call\n");

        data_socket = accept(connection_socket, NULL, NULL);

        if (data_socket == -1)
        {
            perror("accept");
            exit(errno);
        }

        printf("Connection accepted from client\n");
        char* buf[50];
        int strs=0;
        while(strs<50){
            for(int j=0;j<5;j++){               //Storing 50 random strings of length 4, along with indices
            char* str = buffer2[strs];
            //printf("%s\n", buffer[i]);
            char buff[50];
            sprintf(buff,"%s %d",str,strs);
            buf[strs]=buff;
            char* string=buf[strs];
            strcpy(data2,buff);
            
            //string=buf[i];
            //printf("%d\n", atoi(&string[5]));
            int nwrite=write(data_socket,&data2,8);
            printf("Data sent is : %s\n", data2);
            if(nwrite==-1){
                perror("write");
                exit(errno);
            }else{
                strs++;
                if(strs>=50){
                    break;
                }
            }

            
            
        }
        //wait(1);
        char maxId[4];
        int nread=read(data_socket,maxId,4);
        if(nread==-1){
            perror("read");
            exit(errno);
        }
        strs=atoi(maxId)+1;
        printf("Highest ID recieved is %d\n",atoi(maxId));
    }

        break;
}
    close(data_socket);
    return 0;
}