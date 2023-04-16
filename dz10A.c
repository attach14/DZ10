#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
       
int verification = 0;

void handler_1(int sign) {
    verification = 1;
}

void handler_2(int sign) {
    verification = 2;
}

void sendNumber(int other_pid, int number) {
    if (number < 0) {
        kill(other_pid, SIGUSR1);
        while (verification == 0)
        {
            continue;
        }
        if(verification == 2) {
            printf("Error occured\n");
            kill(other_pid, SIGINT);
            return;
        }
        number *= -1;
    } else {
        kill(other_pid, SIGUSR2);
        while (verification == 0)
        {
            continue;
        }
        if(verification == 1) {
            printf("Error occured\n");
            kill(other_pid, SIGINT);
            return;
        }
    }
    verification = 0;
    while(number) {
        int cur = number % 2;
        if (cur == 1) {
            kill(other_pid, SIGUSR1);
        } else {
            kill(other_pid, SIGUSR2);
        }
        while (verification == 0)
        {
            continue;
        }
        if ((cur == 1 && verification == 2) || (cur == 0 && verification == 1)) {
            printf("Error occured\n");
            kill(other_pid, SIGINT);
            return;
        }
        number /= 2;
        verification = 0;
    }
    kill(other_pid, SIGINT);
}

int main( int argc, char *argv[], char *env[] ){
    int my_pid = getpid();
    int other_pid;
    printf("pid = %d\n", my_pid);
    printf("Write other pid: ");
    scanf("%d", &other_pid);
    printf("Write input number: ");
    int number;
    scanf("%d", &number);
    (void) signal(SIGUSR1, handler_1);
    (void) signal(SIGUSR2, handler_2);
    sendNumber(other_pid, number);
    return 0;
}
 
