// for de 4 con un fork

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printMSG(int id){
    if (id == 0) {
        printf("Soy el hijo %d\n", getpid());
    } else {
        printf("Soy el padre %d\n", getpid());
    }
} 

int main() {
    int i;
    for (i = 0; i < 4; i++) {
        i = fork();
        printMSG(i);
    }

    
    return 0;
}