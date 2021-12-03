#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<errno.h>

#define PROJECT_PATHNAME "p1_mesq.c"
#define PROJECT_ID 57
#define MSG_SIZE 512


struct msg_struct
{
    long type;
    char msg[50][8];
}message;


int main()
{
    
    // * set up message queue

    // * Generate unique key to access the message queue
    key_t key = ftok(PROJECT_PATHNAME, PROJECT_ID);
    if(key==-1)
    {
        printf("Error generating msg key!\n%s\n", strerror(errno));
        return -1;
    }

    // * Create queue in kernel or get the id for an existing queue
    int qid = msgget(key, 0666|IPC_CREAT);//? Why 0666?
    if(qid==-1)
    {
        printf("Error retrieving queue id!\n%s\n", strerror(errno));
        return -1;
    }

    struct msqid_ds buf;
    
    // * Receive messages while the queue is not empty

    do
    {
        if(msgrcv(qid, &message, sizeof(message),1,0)==-1)
        {
            printf("Error recieving message!\n%s\n", strerror(errno));
            return -1;
        }
        //printf("%s(%ld) ", message.msg, message.type);
        
        if(msgctl(qid, IPC_STAT, &buf) == -1) // * check queue status
        {
            printf("Error retrieving msgctl info!\n%s\n", strerror(errno));
            return -1;
        }
    }while(buf.msg_qnum!=0);
    printf("\n");
    

    

    
    // for(int i=0; i<50; i++){
    //     msgrcv(qid, &message, sizeof(message),1,0);
    // }      
    
    
    for(int i=0;i<50;i++){
        char* string=message.msg[i];
        printf("Data Received is : %s and id is %d\n", message.msg[i],atoi(&string[5])); 
        //printf("Data Received is : %s\n", message.msg[i]);
    }
//28109
    msgctl(qid, IPC_RMID, NULL); // * remove message queue

    return 0;
}
