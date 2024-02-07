// 4 forks

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void printMSG(int id){
    if (id == 0) {
        printf("Soy el hijo %d\n", getpid());
        sleep(3);
    } else {
        wait(NULL);
        printf("Soy el padre %d\n", getpid());
    }
} 

int main() {
    int i;
    i = fork();
    printMSG(i);

    i = fork();
    printMSG(i);

    i = fork();
    printMSG(i);

    i = fork();
    printMSG(i);

    
    return 0;
}