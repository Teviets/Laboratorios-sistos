#include <stdio.h>
#include <stdlib.h>

void elFor(){
    clock_t start, end;

    start = clock();
    
    for (int i = 0; i < 1000000; i++){
        printf("%d\n", i);
    }

    end = clock();

    double time_taken = ((double)end - start) / (double)1000;
    printf("----------------\n");
    printf("Tiempo de ejecución de for: %f\n", time_taken);

}

int main() {
    clock_t start, end;

    start = clock();

    elFor();
    elFor();
    elFor();

    end = clock();

    double time_taken = ((double)end - start) / (double)1000;
    printf("Tiempo de ejecución: %f\n", time_taken);

    return 0;
}