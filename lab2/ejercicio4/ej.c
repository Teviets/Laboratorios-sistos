#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    pid_t pid;

    pid = fork();

    if (pid == 0){
        for (int i = 0; i < 40000000; i++){
            printf("%d\n", i);
        }
    }else{

        while(1 == 1){
            printf("Soy el padre\n");
            sleep(5);
        }
    }
}