// include stdio.h, time.h, unistd.h, and sys/wait.h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

void elFor(){
    for (int i = 0; i < 1000000; i++){

    }

}

int main(){
    clock_t start, end;
    pid_t pid;

    start = clock();

    pid = fork();

    if (pid == 0){
        pid_t pid2 = fork();
        if (pid2 == 0){
            pid_t pid3 = fork();
            if (pid3 == 0){
                elFor();
            }else {
                wait(NULL);
                elFor();
            }
        }else {
            wait(NULL);
            elFor();
        }
    }else{
        wait(NULL);
        end = clock();

        double time_taken = ((double)end - start) / (double)1000;
        printf("Tiempo de ejecución: %f\n", time_taken);
    }

    return 0;
}