#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

//段错误信号
int main() {
  //int kill(pid_t pid, int sig);
  //向pid进程发送sig信号
  //kill(getpid(), SIGKILL);
  
  //int raise(int sig);
  //向自己发送sig信号
  //raise(15);
  
  //void abort(void);
  //给自己发送SIGABRT信号
  //abort();
  
  //unsigned int alarm(unsigned int seconds);
  //在经过second秒之后，给自己发送一个SIGALRM信号----定时器
  alarm(3);
  //alarm(0);   //取消上一个定时器, 设置一个就会取消上一个时间未到的定时器，也就是只有一个alarm有效
  while(1) {
    printf("nihao~~\n");
    sleep(1);
  }

  char* ptr = NULL;
  memcpy(ptr, "nihao", 5);

  return 0;
}
