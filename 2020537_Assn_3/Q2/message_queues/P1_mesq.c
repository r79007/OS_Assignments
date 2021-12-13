#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<errno.h>
#include <stdint.h>
#include<fcntl.h>

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

// struct msg_struct
// {
//     long type;
//     char msg[50][8];
//     char maxId[10];
// }message2;

int rdrand16_step (uint16_t *rand)
{
	unsigned char ok;

	asm volatile ("rdrand %0; setc %1"
		: "=r" (*rand), "=qm" (ok));

	return (int) ok;
}

char *randomString(size_t length) { 

static char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
char *randomString;

if (length!=0) {
    randomString = malloc(length +1); 

    if (randomString) {
        int sz = (int) (sizeof(characters) -1); 
        int key; 
        for (int i = 0;i < length;i++) {
            uint16_t rndnum;
            rdrand16_step(&rndnum);        
            key = rndnum % sz;   
            randomString[i] = characters[key];
        }

        randomString[length] = '\0';
    }
}

 return randomString;
}

int main(){
    
    char* buffer[50];    
    for(int i=0;i<50;i++){               //Storing 50 random strings of length 4
        buffer[i] = randomString(4);
    }

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

    message.type=1;
    //struct msqid_ds buf2;
    char* buf[50];
    int strs=0;
        for(;;){
            //printf("hi\n");
            for(int j=0;j<5;j++){    
            //printf("hi\n");           //Storing 50 random strings of length 4, along with indices
            char* str = buffer[strs];
            //printf("%s\n", buffer[i]);
            char buff[50];
            sprintf(buff,"%s %d",str,strs);
            buf[strs]=buff;
            char* string=buf[strs];
            strcpy(message.msg[strs],buff);
            //printf("hi1\n");
            msgsnd(qid,&message,sizeof(message),IPC_NOWAIT);
            //printf("%lu\n",buf2.msg_qnum);
            printf("Data sent is : %s\n", message.msg[strs]);
            
                strs++;
                if(strs>=50){
                    break;
                }
            }

        
        if(msgrcv(qid, &message2, sizeof(message2),2,0)==-1)
        {
            printf("Error recieving message!\n%s\n", strerror(errno));
            return -1;
        }else{
            //printf("hi2\n");
        }
            strs=atoi(message2.maxId)+1;
            
            printf("Highest ID recieved is %d\n", atoi(message2.maxId));
            if(strs>=50){
                break;
            }
}
        //wait(1);
    return 0;
}
