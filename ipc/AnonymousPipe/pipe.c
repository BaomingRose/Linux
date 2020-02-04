#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

//实现最基本的管道通信
//体会管道的一些特性

#if 0   //验证：若管道中没有数据，则read会阻塞
int main() {
  //管道必须创建于创建子进程之前(这样子进程才能复制到管道的操作句柄)
  //int pipe(int pipefd[2]);
  int pipefd[2];
  int ret = pipe(pipefd);
  if (ret < 0) {
    perror("pipe error");
    return -1;
  }

  //父进程向管道写数据，子进程从管道读取数据
  int pid = fork();
  if (pid == 0) {
    char buf[1024] = { 0 };
    //为了验证子进程已经运行等待父进程写完再读取
    printf("-------------\n");
    read(pipefd[0], buf, 1023);
    printf("child read buf[%s]\n", buf);
  } else if (pid > 0) {
    //为了验证是否子进程总是等父进程写完数据再读取，让父进程睡一秒
    sleep(1);
    write(pipefd[1], "hello world", 11);
  }

  return 0;
}
#endif

#if 0   //若管道中数据写满了，则write会阻塞
int main() {
  int pipefd[2];
  int ret = pipe(pipefd);
  if (ret < 0) {
    perror("pipe error");
    return -1;
  }

  int pid = fork();
  if (pid == 0) {
    char buf[1024] = { 0 };
    pause();
    printf("-------------\n");
    read(pipefd[0], buf, 1023);
    printf("child read buf[%s]\n", buf);
  } else if (pid > 0) {

    int total_len = 0;
    /* 计算可写入的最大total_len为65536 即64k
       while (1) {
       total_len += write(pipefd[1], "hello world", 1);
       printf("total_len:%d\n", total_len);
       */
  }

  return 0;
  }
#endif

#if 0   //关闭所有写端，read返回0 
  int main() {
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret < 0) {
      perror("pipe error");
      return -1;
    }

    int pid = fork();
    if (pid == 0) {
      //关闭所有写端，不要忘了自己
      close(pipefd[1]);
      char buf[1024] = { 0 };
      //若所有写端被关闭，read不再阻塞，读完数据后返回0
      int ret = read(pipefd[0], buf, 1023);
      printf("child read buf[%d - %s]\n", ret,  buf);
    } else if (pid > 0) {

      close(pipefd[1]);
      int total_len = 0;
      total_len += write(pipefd[1], "hello world", 11);
    }

    return 0;
  }
#endif

#if 1   //关闭所有读端, write会触发异常 
  int main() {
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret < 0) {
      perror("pipe error");
      return -1;
    }

    int pid = fork();
    if (pid == 0) {
      close(pipefd[0]);
      char buf[1024] = { 0 };
      int ret = read(pipefd[0], buf, 1023);
      printf("child read buf[%d - %s]\n", ret,  buf);
      exit(0);
    } else if (pid > 0) {
      //保证子进程先把读端关闭
      sleep(1);
      close(pipefd[0]);
      int total_len = 0;
      total_len += write(pipefd[1], "hello world", 11);
    }
    //如果write异常，不会打印下面
    printf("--------------\n");

    return 0;
  }
#endif
