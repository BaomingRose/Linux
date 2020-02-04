#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main() {
  //ls | grep make
  int pipefd[2];
  int ret = pipe(pipefd);
  if (ret < 0) {
    perror("pipe error");
    return -1;
  }
  int pid1 = fork();
  if (pid1 == 0) {
    //ls
    close(pipefd[0]);
    dup2(pipefd[1], 1);   //将标准输出重定向到管道写入端
    execlp("ls", "ls", NULL);
    //如果替换失败直接退出
    exit(0);
  }
  int pid2 = fork();
  if (pid2 == 0) {
    //grep make
    close(pipefd[1]);   //关闭所有写端，grep循环读取数据，read返回0，就不在读
    dup2(pipefd[0], 0);   //将标准输入端重定向到管道读取端
    execlp("grep", "grep", "make", NULL);
    exit(0);
  }
  close(pipefd[0]);
  //父进程的写端也要关闭
  close(pipefd[1]);
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);

  return 0;
}
