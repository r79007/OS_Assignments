#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<errno.h>

#define PROJECT_PATHNAME "P1_mesq.c"
#define PROJECT_ID 57
#define MSG_SIZE 408


struct msg_struct
{
    long type;
    char msg[50][8];
    char maxId[10];
}message;

struct msg_struct2
{
    long type;
    char msg[50][8];
    char maxId[10];
}message2;


int main()
{
    int k=0;

    key_t key = ftok(PROJECT_PATHNAME, PROJECT_ID);
    if(key==-1)
    {
        printf("Error generating msg key!\n%s\n", strerror(errno));
        return -1;
    }
    int qid = msgget(key, 0666|IPC_CREAT);
    if(qid==-1)
    {
        printf("Error retrieving queue id!\n%s\n", strerror(errno));
        return -1;
    }

    struct msqid_ds buf;
    
    {
        
    };
    
 for(;;){  
     int i=0;
    do
    {
        if(msgrcv(qid, &message, sizeof(message),1,0)==-1)
        {
            printf("Error recieving message!\n%s\n", strerror(errno));
            return -1;
        }else{
            //printf("%lu",buf.msg_qnum);
            sprintf(message2.maxId,"%s",message.msg[k]+5);
            char rec[10];
            strcpy(rec,message.msg[k]);
            char* p;
            p=strtok(rec," ");
            printf("String Received is : %s and it's id is %s\n", p,message.msg[k]+5);
            k++;
            i++;
        }
        //printf("%s(%ld) ", message.msg, message.type);
        
        if(msgctl(qid, IPC_STAT, &buf) == -1) // * check queue status
        {
            printf("Error retrieving msgctl info!\n%s\n", strerror(errno));
            return -1;
        }
    }while(k<50 && i<5);
    //printf("\n");

    i=0;
    message2.type=2;
    //printf("hi11\n");
    msgsnd(qid,&message2,sizeof(message2),IPC_NOWAIT);
    //printf("hi12\n");
    //message.type=1;
    if(k>=50){
        break;
    }

 }
    
    // for(int i=0; i<50; i++){
    //     msgrcv(qid, &message, sizeof(message),1,0);
    // }      
    
    
    // for(int i=0;i<50;i++){
    //     char* string=message.msg[i];
    //     printf("Data Received is : %s and id is %d\n", message.msg[i],atoi(&string[5])); 
    //     //printf("Data Received is : %s\n", message.msg[i]);
    // }
//28109
    msgctl(qid, IPC_RMID, NULL);

    return 0;
}
