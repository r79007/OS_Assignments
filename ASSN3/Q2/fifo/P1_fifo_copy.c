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

char *randomString(size_t length) { 

static char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // could be const
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
        buffer2[i] = randomString(4);
        //printf("%s\n", buffer[i]);
    }
    



for (;;)
    {

        
        data_socket=open("fifo1",O_WRONLY);
        int data_socket2=open("fifo2",O_RDONLY);
                if(data_socket2==-1){
                    perror("open");
                    exit(errno);
                }
        
        if(data_socket==-1){
        perror("open");
        exit(errno);
    }
        char* buf[50];
        int strs=0;
        while(strs<50){
            for(int j=0;j<5;j++){               
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

                if(j!=4){
                char maxId[8];
                int nread=read(data_socket2,maxId,8);
                if(nread==-1){
                    perror("read");
                    exit(errno);
                }
                // strs=atoi(maxId)+1;
                //printf("Highest ID recieved is %d\n", atoi(maxId));
            }

            }

            
            
        }

        char maxId[8];
                int nread=read(data_socket2,maxId,8);
                if(nread==-1){
                    perror("read");
                    exit(errno);
                }
                strs=atoi(maxId)+1;
                printf("Highest ID recieved is %d\n", atoi(maxId));
        //wait(1);
        // int data_socket2=open("fifo2",O_RDONLY);
        // if(data_socket2==-1){
        //     perror("open");
        //     exit(errno);
        // }
        // char maxId[8];
        // int nread=read(data_socket2,maxId,8);
        // if(nread==-1){
        //     perror("read");
        //     exit(errno);
        // }
        // strs=atoi(maxId)+1;
        // printf("Highest ID recieved is %d\n", atoi(maxId));
    }

        break;
}

    close(data_socket);
    


    return 0;
}