#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//实现信号阻塞, 体会信号的可靠与不可靠
//先将所有信号全部阻塞
//getchar 在用户按下回车之前，这些信号一直被阻塞，之后解除阻塞---这时候解除阻塞，将被处理
//在回车之前使用多个ctrl+c和对个kill -40 然后按回车观察结果

void sigcb(int signo) {
  printf("recv signo:%d\n", signo);
}

int main() {
  //非可靠信号
  signal(SIGINT, sigcb);
  //可靠信号
  signal(40, sigcb);
  //int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
  //how：
  //    SIG_BLOCK   向阻塞集合中加入set集合中的信号 block = mask | set
  //    SIG_UNBLOCK   从阻塞集合中移除set集合中的信号 block = mask & (~set)
  //    SIG_SETMASK   将set集合中的信号设置为阻塞集合 block = set
  //oldset  用于保存修改前，阻塞集合中的信号
  sigset_t set, oldset;
  sigemptyset(&set);
  //int sigfillset(sigset_t *set);
  //向set集合中添加所有信号
  //int sigaddset(sigset_t *set, int signum);
  //向set集合中添加signum信号
  sigfillset(&set);
  sigprocmask(SIG_BLOCK, &set, &oldset);  //阻塞set中的信号

  getchar();

  sigprocmask(SIG_UNBLOCK, &set, NULL);
  //sigprocmask(SIG_SETMASK, &oldset, NULL);

  return 0;
}
