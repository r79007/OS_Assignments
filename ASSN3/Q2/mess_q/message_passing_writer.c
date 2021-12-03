#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<errno.h>

#define PROJECT_PATHNAME "message_passing_writer.c"
#define PROJECT_ID 57
#define MSG_SIZE 512


struct msg_struct
{
    long type;
    char msg[512];
}message;


int main()
{
    FILE *fp;
    // message to break up into words and send to the message queue
    char buff[512] = "Hello, this the writer process writing to the message queue!\n";

    //set up message queue

    key_t key = ftok(PROJECT_PATHNAME, PROJECT_ID);
    if(key==-1)
    {
        printf("Error generating msg key!\n%s", strerror(errno));
        return -1;
    }

    int qid = msgget(key, 0666|IPC_CREAT);
    if(qid==-1)
    {
        printf("Error retrieving queue id!\n%s", strerror(errno));
        return -1;
    }



    char token[512];
    int index=0;
    int msg_type = 1;

    for(int i=0; buff[i]!='\0'; i++)
    {
        if(buff[i]==' ' || buff[i]=='\n')//space or newline
        {
            token[index++]='\0'; // null terminator
            //send data
            // * Change the message type field
            strcpy(message.msg, token);
            message.type = msg_type++;
            if(msgsnd(qid, &message, MSG_SIZE, 0)==-1)
            {
                printf("Error sending message!\n%s", strerror(errno));
                return -1;
            }
            index=0;
        }
        else
        {
            token[index++] = buff[i];
        }
    }


    return 0; // exit program

}
