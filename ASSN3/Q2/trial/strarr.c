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

#define PROJECT_PATHNAME "p1_mesq.c"
#define PROJECT_ID 57
#define MSG_SIZE 512

struct msg_struct
{
    long type;
    char msg[512];
}message;

int rdrand16_step (uint16_t *rand)
{
	unsigned char ok;

	asm volatile ("rdrand %0; setc %1"
		: "=r" (*rand), "=qm" (ok));

	return (int) ok;
}

char *gen_random(size_t length) { // const size_t length, supra

static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // could be const
char *randomString;

if (length) {
    randomString = malloc(length +1); // sizeof(char) == 1, cf. C99

    if (randomString) {
        int l = (int) (sizeof(charset) -1); // (static/global, could be const or #define SZ, would be even better)
        int key;  // one-time instantiation (static/global would be even better)
        for (int n = 0;n < length;n++) {
            uint16_t rndnum;
            rdrand16_step(&rndnum);        
            key = rndnum % l;   // no instantiation, just assignment, no overhead from sizeof
            randomString[n] = charset[key];
        }

        randomString[length] = '\0';
    }
}

return randomString;
}

int main(){
    
    char* buffer[50];    
    for(int i=0;i<50;i++){               //Storing 50 random strings of length 4
        buffer[i] = gen_random(4);
        //printf("%s\n", buffer[i]);
    }



    char* buf[50];
    char arr[400];
    for(int j=0;j<50;j++){               //Storing 50 random strings of length 4, along with indices
        char* str = buffer[j];
        //printf("%s\n", buffer[i]);
        char buff[50];
        sprintf(buff,"%s %d",str,j);
        buf[j]=buff;
        char* string=buf[j];
        //strcpy(message.msg+j*8,string);
        strcpy(arr+j*8,buff);
        printf("%s\n",arr+j*8);
        //string=buf[i];
        //printf("%d\n", atoi(&string[5]));
        //msgsnd(qid, (int *) &message.a[i], sizeof(message.a[i]), 0);

    }



    return 0;
}
