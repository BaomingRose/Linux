#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  int pid = vfork();
  if (0 == pid) {
    printf("i am child!\n");
    sleep(5);
    //exit(0);
    return -1;
    printf("i am child two!\n");
  }
  printf("i am parent!\n");

  return 0;
}
