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

#define SOCKET_NAME "/tmp/DemoSocket"
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
{ // const size_t length, supra

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

#if 0  
    struct sockaddr_un {
        sa_family_t sun_family;               /* AF_UNIX */
        char        sun_path[108];            /* pathname */
    };
#endif

    int ret;
    int connection_socket;
    int data_socket;
    int result;
    int data;
    char buffer[BUFFER_SIZE];

    /*In case the program exited inadvertently on the last run,
     *remove the socket.
     **/

    unlink(SOCKET_NAME);

    /* Create Master socket. */

    /*SOCK_DGRAM for Datagram based communication*/
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (connection_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Master socket created\n");

    /*initialize*/
    memset(&name, 0, sizeof(struct sockaddr_un));

    /*Specify the socket Cridentials*/
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    /* Bind socket to socket name.*/
    /* Purpose of bind() system call is that application() dictate the underlying 
     * operating system the criteria of recieving the data. Here, bind() system call
     * is telling the OS that if sender process sends the data destined to socket "/tmp/DemoSocket", 
     * then such data needs to be delivered to this server process (the server process)*/
    ret = bind(connection_socket, (const struct sockaddr *)&name,
               sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("bind() call succeed\n");
    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     * */

    ret = listen(connection_socket, 20);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */
    /*All Server process usually runs 24 x 7. Good Servers should always up
     * and running and shold never go down. Have you ever seen Facebook Or Google
     * page failed to load ??*/
    for (;;)
    {

        /* Wait for incoming connection. */
        printf("Waiting on accept() sys call\n");

        data_socket = accept(connection_socket, NULL, NULL);

        if (data_socket == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
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
            //msgsnd(qid, (int *) &message.a[i], sizeof(message.a[i]), 0);
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
        strs=atoi(maxId)+1;

    }

        break;
}
    close(data_socket);
    return 0;
}