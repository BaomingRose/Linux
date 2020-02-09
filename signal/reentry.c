#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//体会函数的重入造成的影响

int a = 1;
int b = 1;

int sum(int* x , int* y) {
  (*x)++;
  sleep(3);
  (*y)++;
  return *x + *y;
}

void sigcb(int signo) {
  printf("signal---------%d\n", sum(&a, &b));
}

int main() {
  signal(SIGINT,sigcb);
  printf("main-------------%d\n", sum(&a, &b));

  return 0;
}
