#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <sys/wait.h>

int main() {
  pid_t pid = fork();
  if (pid < 0) {
    //errno是一个全局变量，存储每次系统调用出现的错误原因
    //strerror,通过错误编号获取字符串错误原因
    printf("fork error : %s\n", strerror(errno));
    //直接打印上一个系统调用错误原因
    perror("fork error");
  } else if (0 == pid) {
    sleep(3);
    exit(111);
  }
  //pid_t wait(int *status);
  //阻塞等待任意一个子进程退出，获取返回值
  //wait(NULL);
  
#if 0
  //pid_t waitpid(pid_t pid, int *status, int options);
  //阻塞等待任意一个子进程或者指定的子进程退出
  //pid : -1 等待任意一个子进程  pid > 0 等待指定的子进程
  //options:  WNOHANG 将waitpid设置为非阻塞;  0 默认阻塞
  //返回值：若WNOHANG被指定，没有子进程退出则立即报错返回0；错误：返回-1
  waitpid(pid, NULL, WNOHANG);    //父进程的fork返回子进程的pid，可以直接将pid给进来
  //如果直接向上面这样使用WNOHANG，并没有重新判断，子进程仍然是僵尸进程
#endif
  //使用waitpid要使用while循环
  int status;
  while (waitpid(pid, &status, WNOHANG) == 0) {
    //非阻塞的轮询操作
    printf("drink coffee!\n");
    sleep(1); 
  }
  if ((status & 0x7f) == 0) {
    printf("exit code:%d\n", (status >> 8) & 0xff);
  }

  if (WIFEXITED(status)) {
    printf("exit code:%d\n", WEXITSTATUS(status));
  }

  while (1) {
    printf("i am parent!!!\n");
    sleep(1);
  }

  return 0;
}
