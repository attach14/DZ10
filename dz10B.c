#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
       
int verification = 0;
int finish = 0;
int neg = 0;
void handler_1(int sign) {
    verification = 1;
}

void handler_2(int sign) {
    verification = 2;
}

void handler_3(int sign) {
    finish = 1;
}

int receiveNumber(int other_pid) {
    int number = 0;
    int last = 1;
    int cnt = 0;
    while(1) {
        while (verification == 0)
        {
            if (finish == 1) {
                if (neg == 1) {
                    number *= -1;
                }
                return number;
            }
            continue;
        }
        cnt++;
        if(cnt == 1) {
            if(verification == 1) {
                neg = 1;
                kill(other_pid, SIGUSR1);
            } else {
                kill(other_pid, SIGUSR2);
            }
            verification = 0;
            continue;
        } 
        if(verification == 1) {
            number += last;
            last *= 2;
            kill(other_pid, SIGUSR1);
        } else {
            last *= 2;
            kill(other_pid, SIGUSR2);
        }
        verification = 0;
    }
}

int main( int argc, char *argv[], char *env[] ){
    int my_pid = getpid();
    int other_pid;
    printf("pid = %d\n", my_pid);
    printf("Write other pid: ");
    scanf("%d", &other_pid);
    (void) signal(SIGUSR1, handler_1);
    (void) signal(SIGUSR2, handler_2);
    (void) signal(SIGINT, handler_3);
    printf("%d\n",receiveNumber(other_pid));
    return 0;
}
 
