#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//体会线程退出
//return :不能在main中return （所有线程退出）
//void pthread_exit(void *retval);  //退出线程自身，谁调用谁退出  retval：线程的退出返回值
//int pthread_cancel(pthread_t thread); //取消其他进程  thread：要取消的线程id

void* thr_entry(void* arg) {
  pthread_cancel((pthread_t)arg);
  //pthread_exit(NULL);
  
  //所有线程getpid都是首线程的id
  printf("----------------%d\n", getpid());
  while (1) {
    printf("i am common thread ----%s\n", (char*)arg);
    sleep(1);
  }

  return NULL;
}

int main() {
  printf("---------------main:%d\n", getpid());

  pthread_t mtid;
  //pthread_t pthread_self(void);
  //获取调用线程的线程id
  mtid = pthread_self();
  pthread_t tid;
  char* param = "this is input param";
  int ret = pthread_create(&tid, NULL, thr_entry, (void*)mtid);
  if (ret != 0) {
    printf("pthread create error\n");
    return -1;
  }

  printf("tid: %p \n", tid);   //无符号长整型%lu
  //pthread_cancel(tid);
  while (1) {
    printf("i am main thread -----\n");
    sleep(1);
  }

  return 0;
}
