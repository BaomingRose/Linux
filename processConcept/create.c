#include<stdio.h>
#include<unistd.h>

//子进程先于父进程退出,子进程成为僵尸进程
//没有读取到子进程退出的返回代码时就会产生僵死(尸)进程

//进程之间数据独有
//写时拷贝技术，子进程复制了父进程的pcb，子进程修改数据时重新通过页表映射到物理内存
int g_val = 100;

int main() {
  printf("hello hello, %d\n", getpid());
  //创建子进程，父进程返回子进程的pid，子进程返回0
  //pid_t fork(void);
  //success:返回两次
  //        返回大于0的子进程的pid --> 父进程
  //        返回0 给 ---> 子进程
  pid_t pid = fork();
  if (pid < 0) {
    printf("fork error\n");
    return -1;
  } else if (pid == 0) {
    //子进程更改全局变量
    g_val = 200;
    //虚拟地址空间相同，但是物理内存不同，所以打印的全局变量不同
    printf("-----------i am child------------%d   g_val:%d    %p\n", getpid(), g_val, &g_val);
  } else {
    sleep(1);
    printf("-----------i am parent-----------%d   g_val:%d    %p\n", getpid(), g_val, &g_val);
  }
  printf("leihoua~~~  %d\n", getpid());
  
  return 0;
}
