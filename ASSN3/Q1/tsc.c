#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <sched.h>
#include <errno.h>


// struct timespec {
//                time_t tv_sec;    /* seconds */
//                long   tv_nsec;   /* nanoseconds */
//            };

int main(int argc, char *argv){
    struct timespec *tp;
    if(sched_rr_get_interval(argv[1],tp)!=-1){
        time_t tim=tp->tv_sec;
        long sec=tp->tv_nsec;
        printf("%ld seconds elapsed and nanosec elapsed %li", tim, sec);
    }else{
        perror("sched_rr_get_interval");
        exit(errno);
    }

    return 0;
}