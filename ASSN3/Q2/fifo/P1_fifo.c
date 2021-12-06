#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include<time.h>
#include <stdint.h>

char data2[10];

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

int main(int argc, char* argv[]) {

    int data_socket;

    if (mkfifo("fifo1", 0777) == -1) {
        if (errno != EEXIST) {
            printf("Could not create fifo file\n");
            return 1;
        }
    }

    char* buffer2[50];    
    for(int i=0;i<50;i++){               //Storing 50 random strings of length 4
        buffer2[i] = gen_random(4);
        //printf("%s\n", buffer[i]);
    }
    



for (;;)
    {

        /* Wait for incoming connection. */
        data_socket=open("fifo1",O_WRONLY);
        
        if(data_socket==-1){
        perror("open");
        exit(errno);
    }
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
        int data_socket2=open("fifo2",O_RDONLY);
        if(data_socket2==-1){
            perror("open");
            exit(errno);
        }
        char maxId[8];
        int nread=read(data_socket2,maxId,8);
        if(nread==-1){
            perror("read");
            exit(errno);
        }
        strs=atoi(maxId)+1;

    }

        break;
}

    close(data_socket);
    


    return 0;
}