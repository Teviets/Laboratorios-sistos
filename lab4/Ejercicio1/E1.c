#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Definir los recursos y mutex
pthread_mutex_t mutex_recurso1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_recurso2 = PTHREAD_MUTEX_INITIALIZER;

// Proceso 1 adquiere recurso1 y luego recurso2
void* proceso1(void* arg) {
    while (1) {
        printf("Proceso 1 intenta adquirir recurso1\n");
        pthread_mutex_lock(&mutex_recurso1);
        printf("Proceso 1 adquirió recurso1\n");
        sleep(3000);  // Simula el trabajo con recurso1

        printf("Proceso 1 intenta adquirir recurso2\n");
        pthread_mutex_lock(&mutex_recurso2);
        printf("Proceso 1 adquirió recurso2\n");
        sleep(3000);  // Simula el trabajo con recurso2
        printf("Proceso 1 liberó recurso2\n");
        pthread_mutex_unlock(&mutex_recurso2);
        printf("Proceso 1 liberó recurso1\n");
        pthread_mutex_unlock(&mutex_recurso1);
    }
}

// Proceso 2 adquiere recurso2 y luego recurso1 (invierte el orden)
void* proceso2(void* arg) {
    while (1) {
        printf("Proceso 2 intenta adquirir recurso2\n");
        pthread_mutex_lock(&mutex_recurso2);
        printf("Proceso 2 adquirió recurso2\n");
        sleep(2000);  // Simula el trabajo con recurso2
        printf("Proceso 2 intenta adquirir recurso1\n");
        pthread_mutex_lock(&mutex_recurso1);
        printf("Proceso 2 adquirió recurso1\n");
        sleep(2000);  // Simula el trabajo con recurso1
        printf("Proceso 2 liberó recurso1\n");
        pthread_mutex_unlock(&mutex_recurso1);
        printf("Proceso 2 liberó recurso2\n");
        pthread_mutex_unlock(&mutex_recurso2);
    }
}

int main() {
    // Crear hilos
    pthread_t thread_proceso1, thread_proceso2;
    pthread_create(&thread_proceso1, NULL, proceso1, NULL);
    pthread_create(&thread_proceso2, NULL, proceso2, NULL);

    // Esperar a que los hilos terminen (esto nunca sucederá debido al deadlock)
    pthread_join(thread_proceso1, NULL);
    pthread_join(thread_proceso2, NULL);

    return 0;
}
