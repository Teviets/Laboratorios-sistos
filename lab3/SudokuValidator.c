#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/syscall.h>

// Array de tablero de sudoku
int board[9][9];

#define MAX_FILENAME_LENGTH 100
#define BUFFER_SIZE 1024

// verificar que cada columna sea validad para cada valor posible en board de un sudoku sin necesidad de pasar algun parametro
void *checkColThread(void *arg) {
    printf("Thread ID: %ld\n", syscall(SYS_gettid));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int number = board[i][j];
            if (number == -1) continue;

            for (int x = 0; x < 9; x++) {
                if (board[x][j] == number) {
                    return (void *)false;
                }
            }
        }
    }
    sleep(3);
    return (void *)true;
}
// verificar que cada fila sea validad para cada valor posible en board de un sudoku sin necesidad de pasar algun parametro
void *checkRowThread(void *arg) {
    printf("Thread ID: %ld\n", syscall(SYS_gettid));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int number = board[i][j];
            if (number == -1) continue;

            for (int y = 0; y < 9; y++) {
                if (board[i][y] == number) {
                    return (void *)false;
                }
            }
        }
    }
    sleep(3);
    return (void *)true;
}

// verificar que cada cuadrante sea validad para cada valor posible en board de un sudoku sin necesidad de pasar algun parametro
void *checkSquareThread(void *arg) {
    printf("Thread ID: %ld\n", syscall(SYS_gettid));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int number = board[i][j];
            if (number == -1) continue;

            int startX = 3 * (i / 3);
            int startY = 3 * (j / 3);

            for (int x = startX; x < startX + 3; x++) {
                for (int y = startY; y < startY + 3; y++) {
                    if (board[x][y] == number) {
                        return (void *)false;
                    }
                }
            }
        }
    }
    sleep(3);
    return (void *)true;
}

// verificar que no se repitan por columnas en el board
bool checkCol(int number, int xpos, int ypos) {
    if (board[xpos][ypos] != -1) return false;

    for (int x = 0; x < 9; x++) {
        if (board[x][ypos] == number) {
            return false;
        }
    }
    printf("Valido por columnas\n");
    return true;
}

// verificar que no se repitan por filas en el board
bool checkRow(int number, int xpos, int ypos) {
    if (board[xpos][ypos] != -1) return false;

    for (int y = 0; y < 9; y++) {
        if (board[xpos][y] == number) {
            return false;
        }
    }
    printf("Valido por filas\n");
    return true;
}

// verificar que no se repitan en el cuadrante 3x3
bool checkSquare(int number, int xpos, int ypos) {
    if (board[xpos][ypos] != -1) return false;

    int startX = 3 * (xpos / 3);
    int startY = 3 * (ypos / 3);

    for (int x = startX; x < startX + 3; x++) {
        for (int y = startY; y < startY + 3; y++) {
            if (board[x][y] == number) {
                return false;
            }
        }
    }
    printf("Valido por cuadrante\n");
    return true;
}

// funcion que genere el board
void loadBoardFromFile(int archivo) {
    char buffer[2]; // Buffer to hold each character read from the file
    int i = 0, j = 0; // Indices for the board

    // Read the file character by character
    while (read(archivo, buffer, 1) > 0) {
        // If the character is a newline, move to the next row
        if (buffer[0] == '\n') {
            i++;
            j = 0;
        }
        // If the character is a space, ignore it
        else if (buffer[0] == ' ') {
            continue;
        }
        // Otherwise, convert the character to an integer and store it in the board
        else {
            board[i][j] = buffer[0] - '0';
            j++;
        }
    }
}

void printBoard(){
    for (int i = 0; i<9; i++){
        for (int j = 0; j<9; j++){
            printf("|%d| ", board[i][j]);
        }
        printf("\n");
        printf(" -----------------------------------");
        printf("\n");
    }
}

// verificar que el board sea valido y que imprima en donde esta el error
void checkBoard() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int number = board[i][j];
            if (number == -1) continue;

            if (checkRow(number, i, j) || checkCol(number, i, j) || checkSquare(number, i, j)) {
                printf("Error en la fila %d, columna %d\n", i, j);
            }
        }
    }
}

int main() {
    char filename[MAX_FILENAME_LENGTH];
    printf("Ingrese el nombre del archivo: \n");
    fgets(filename, MAX_FILENAME_LENGTH, stdin);

    if (filename[strlen(filename) - 1] == '\n') {
        filename[strlen(filename) - 1] = '\0';
    }

    int archivo = open(filename, O_RDONLY);

    if (archivo == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }

    loadBoardFromFile(archivo);
    checkBoard();
    printBoard();

    // Obtener el número de proceso (PID) del proceso padre
    pid_t parentPID = getpid();

    // Crear un proceso hijo
    pid_t childPID = fork();

    if (childPID == -1) {
        perror("Error en fork");
        return 1;
    }

    if (childPID == 0) {
        // En el proceso hijo

        // Convertir el número del proceso padre a texto
        char parentPIDString[20];
        snprintf(parentPIDString, sizeof(parentPIDString), "%d", parentPID);

        // Ejecutar el comando con execlp
        execlp("ps", "ps", "-p", parentPIDString, "-lLf", (char *)NULL);

        // Si execlp falla
        perror("Error en execlp");
        exit(EXIT_FAILURE);
    } else {
        
        pthread_t threadCol;
        
        pthread_create(&threadCol, NULL, checkColThread, NULL);
        pthread_join(threadCol, NULL);
        syscall(SYS_gettid); // id del thread

        
        

        bool row;
        // obtener el resultado de checkrow en cada posicion posible
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                row = checkRow(board[i][j], i, j);
            }
        }

        bool square;
        // obtener el resultado de checkSquare en cada posicion posible
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                square = checkSquare(board[i][j], i, j);
            }
        }

        if (row && square) {
            printf("El tablero es valido\n");
        } else {
            printf("El tablero no es valido\n");
        }
        // En el proceso padre, esperar a que el hijo termine
        wait(NULL);
    }

    close(archivo);
    return 0;
}


