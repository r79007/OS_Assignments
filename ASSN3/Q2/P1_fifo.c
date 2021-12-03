#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/types.h>
#include<fcntl.h>

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

int main(int argc, char *argv[]){
    
    char* buffer[50];    
    for(int i=0;i<50;i++){               //Storing 50 random strings of length 4
        buffer[i] = gen_random(4);
        //printf("%s\n", buffer[i]);
    }


    if (mkfifo("fifo2", 0777) == -1) {
        if (errno != EEXIST) {
            printf("Could not create fifo file\n");
            return 1;
        }
    }

    int fd=open("fifo1",O_WRONLY);
    if(fd==-1){
        perror("open");
        exit(errno);
    }


    char* buf[50];
    for(int j=0;j<50;j++){               //Storing 50 random strings of length 4
        char* str = buffer[j];
        //printf("%s\n", buffer[i]);
        char buff[50];
        sprintf(buff,"%s %d",str,j);
        buf[j]=buff;
        char* string=buf[j];
        //string=buf[i];
        //printf("%d\n", atoi(&string[5]));
    }

    if(write(fd,buf,50*sizeof(buf[15]))==-1){
        perror("write");
        exit(errno);
    }

    close(fd);
    


    return 0;
}