#include <stdio.h>
#include <unistd.h>

int main() {
  //获取当前进程pid
  printf("%d\n", getpid());
  //获取父进程pid
  printf("%d\n", getppid());
  while (1) {
    printf("hello ------- %d\n", getpid());
  }

  return 0;
}
