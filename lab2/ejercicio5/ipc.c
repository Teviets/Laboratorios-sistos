#include <stdio.h>
#include <stdlib.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

  int n = atoi(argv[1]);
  char x = argv[2][0];

  int shmid;
  key_t key = 1234;
  
  if ((shmid = shmget(key, n, IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    exit(1);
  } else {
    printf("%c: Created new shared mem obj %d\n", x, shmid);
  }

  void *shm = shmat(shmid, NULL, 0);
  if (shm == (void *) -1) {
    perror("shmat");
    exit(1);
  } else {
    printf("%c: Ptr created with value %p\n", x, shm); 
  }

  printf("%c: Initialized shared mem obj\n", x);

  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) { // child
    char c;
    do {
      read(STDIN_FILENO, &c, 1);
      if (c != '#') {
        ((char *)shm)[0] = c;
      }
    } while (c != '#');
  } else { // parent
    for (int i = 0; i < n; i++) {
      if (i % n == 0) {
        write(STDOUT_FILENO, &x, 1);
      }
    }

    write(STDOUT_FILENO, "#", 1);
    wait(NULL);
    printf("%c: Shared memory has: ", x);
    for (int i = 0; i < n; i++) {
      printf("%c", ((char *)shm)[i]);
    }
    printf("\n");
  }

  shmdt(shm); 
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}