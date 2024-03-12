#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

int available[MAX_RESOURCES];
int max_claim[MAX_PROCESSES][MAX_RESOURCES];
int current_allocation[MAX_PROCESSES][MAX_RESOURCES];
int max_need[MAX_PROCESSES][MAX_RESOURCES];

bool finish[MAX_PROCESSES];

bool check_safety() {
    int work[MAX_RESOURCES];
    for (int i = 0; i < MAX_RESOURCES; ++i) {
        work[i] = available[i];
    }

    bool finish_copy[MAX_PROCESSES];
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        finish_copy[i] = finish[i];
    }

    while (true) {
        bool found = false;
        for (int i = 0; i < MAX_PROCESSES; ++i) {
            if (!finish_copy[i]) {
                bool can_allocate = true;
                for (int j = 0; j < MAX_RESOURCES; ++j) {
                    if (max_need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                if (can_allocate) {
                    // Allocate resources to the process
                    for (int j = 0; j < MAX_RESOURCES; ++j) {
                        work[j] += current_allocation[i][j];
                    }
                    finish_copy[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            break;
        }
    }

    // Check if all processes are finished
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if (!finish_copy[i]) {
            return false;
        }
    }

    return true;
}

bool request_resources(int process_id, int request[]) {
    // Check if request is within max_claim
    for (int i = 0; i < MAX_RESOURCES; ++i) {
        if (request[i] > max_need[process_id][i]) {
            return false;  // Request exceeds max_need
        }
    }

    // Check if request can be satisfied
    for (int i = 0; i < MAX_RESOURCES; ++i) {
        if (request[i] > available[i]) {
            return false;  // Request cannot be satisfied immediately
        }
    }

    // Pretend to allocate resources
    for (int i = 0; i < MAX_RESOURCES; ++i) {
        available[i] -= request[i];
        current_allocation[process_id][i] += request[i];
        max_need[process_id][i] -= request[i];
    }

    // Check if the system remains in a safe state after the allocation
    if (check_safety()) {
        return true;
    } else {
        // Rollback the allocation
        for (int i = 0; i < MAX_RESOURCES; ++i) {
            available[i] += request[i];
            current_allocation[process_id][i] -= request[i];
            max_need[process_id][i] += request[i];
        }
        return false;
    }
}

void release_resources(int process_id) {
    for (int i = 0; i < MAX_RESOURCES; ++i) {
        available[i] += current_allocation[process_id][i];
        current_allocation[process_id][i] = 0;
        max_need[process_id][i] = max_claim[process_id][i];
    }
}

int main() {
    // Initialize available resources
    for (int i = 0; i < MAX_RESOURCES; ++i) {
        available[i] = rand() % 10 + 1;
    }

    // Initialize max_claim matrix with random values
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            max_claim[i][j] = rand() % 5 + 1;
        }
    }

    // Initialize max_need matrix
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            max_need[i][j] = max_claim[i][j];
        }
    }

    // Simulate resource requests and releases
    for (int i = 0; i < 10; ++i) {
        int process_id = rand() % MAX_PROCESSES;

        int request[MAX_RESOURCES];
        for (int j = 0; j < MAX_RESOURCES; ++j) {
            request[j] = rand() % (max_need[process_id][j] + 1);
        }

        if (request_resources(process_id, request)) {
            printf("Process %d requests resources: ", process_id);
            for (int j = 0; j < MAX_RESOURCES; ++j) {
                printf("%d ", request[j]);
            }
            printf("=> Resource allocation successful.\n");
        } else {
            printf("Process %d requests resources: ", process_id);
            for (int j = 0; j < MAX_RESOURCES; ++j) {
                printf("%d ", request[j]);
            }
            printf("=> Resource allocation failed.\n");
        }

        int process_to_release = rand() % MAX_PROCESSES;
        release_resources(process_to_release);
        printf("Process %d releases resources.\n", process_to_release);
    }

    return 0;
}
