#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//体会信号替换动作的修改
//typedef void (*sighandler_t)(int);
//sighandler_t signal(int signum, sighandler_t handler);
//  signum：信号编号--替换signum这个信号的处理函数
//  handler：（函数指针）用户传入的处理函数
//      SIG_DFL：信号的默认处理动作
//      SIG_IGN：信号的忽略处理动作
//int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

void sigcb(int signo) {
  printf("recv a signo:%d\n", signo);
}

int main() {
  //typedef void (*sighandler_t)(int);  //函数指针
  //sighandler_t signal(int signum, sighandler_t handler);
  //  signum：信号编号--替换signum这个信号的处理函数
  //  handler：（函数指针）用户传入的处理函数
  //      SIG_DFL：信号的默认处理动作
  //      SIG_IGN：信号的忽略处理动作
  //signal(SIGINT, SIG_IGN);  //让2号信号忽略处理

  //int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
  /*struct sigaction {
       void     (*sa_handler)(int);
       void     (*sa_sigaction)(int, siginfo_t *, void *);
       sigset_t   sa_mask;
       int        sa_flags;
       void     (*sa_restorer)(void);
   }; */
  //}
  struct sigaction new_act, old_act;
  new_act.sa_flags = 0;
  //new_act.sa_handler = SIG_IGN;
  new_act.sa_handler = sigcb;
  //int sigemptyset(sigset_t *set);
  //清空set信号集合
  sigemptyset(&new_act.sa_mask);

  //使用new_act替换2号信号的处理动作，将原有动作保存到old_act中
  sigaction(2, &new_act, &old_act);
  
  while (1) {
    printf("hello~~\n");
    //信号会打断当前的阻塞操作，打断之后直接进入下一次循环
    sleep(10);
  }
  
  return 0;
}
