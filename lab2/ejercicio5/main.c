// exec.c
#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    return 1;
  }

  if (pid == 0){
    printf("I am a\n");
    execl("./ipc", "ipc", "10", "a", NULL);
  }else{
    printf("I am b\n");
    execl("./ipc", "ipc", "10", "b", NULL);
  }
  

  

  return 0;
}