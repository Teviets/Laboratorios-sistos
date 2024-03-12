#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5

pthread_mutex_t tenedores[NUM_FILOSOFOS];
sem_t sem_filosofos;

void pensar(int filosofo_id) {
    printf("Filósofo %d está pensando.\n", filosofo_id);
    sleep(2);  // Simulación de pensamiento
}

void comer(int filosofo_id) {
    printf("Filósofo %d está comiendo.\n", filosofo_id);
    sleep(2);  // Simulación de comer
}

void *filosofo(void *arg) {
    int filosofo_id = *((int *)arg);
    while (1) {
        pensar(filosofo_id);

        // Espera a que el semáforo esté disponible
        sem_wait(&sem_filosofos);

        // Bloquea el tenedor de la izquierda
        pthread_mutex_lock(&tenedores[filosofo_id]);
        printf("Filósofo %d tiene el tenedor izquierdo.\n", filosofo_id);

        // Bloquea el tenedor de la derecha
        pthread_mutex_lock(&tenedores[(filosofo_id + 1) % NUM_FILOSOFOS]);
        printf("Filósofo %d tiene el tenedor derecho.\n", filosofo_id);

        // Libera el semáforo para permitir que otros filósofos accedan a los tenedores
        sem_post(&sem_filosofos);

        comer(filosofo_id);

        // Libera los tenedores después de comer
        pthread_mutex_unlock(&tenedores[filosofo_id]);
        pthread_mutex_unlock(&tenedores[(filosofo_id + 1) % NUM_FILOSOFOS]);
    }
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    // Inicializa el semáforo
    sem_init(&sem_filosofos, 0, NUM_FILOSOFOS - 1);

    // Inicializa los mutex de los tenedores
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&tenedores[i], NULL);
    }

    // Crea los hilos de los filósofos
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);
    }

    // Espera a que los hilos terminen (esto nunca sucederá en este ejemplo infinito)
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    // Libera los recursos
    sem_destroy(&sem_filosofos);
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&tenedores[i]);
    }

    return 0;
}
