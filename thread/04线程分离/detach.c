#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void* thr_entry(void* arg) {
  pthread_detach(pthread_self());
  while (1) {
    printf("i am common thread-----%s\n", (char*)arg);
    sleep(1);
  }
  pthread_exit("nihao~~\n");
  
  return NULL;
}

int main() {
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

  //int pthread_detach(pthread_t thread);
  //是将线程joinable属性修改为detach属性，
  //线程若处于detach属性，则线程退出后将自动回收资源；
  //并且这个线程不需要被等待，等待是毫无意义的因为线程退出返回值占用的空间已经被回收了
  pthread_detach(tid);
  //ret = pthread_detach(tid);
  if (ret == EINVAL) {
    printf("thread is not joinable\n");
  }
  printf("tid: %p -------  \n", tid);   //无符号长整型%lu
  while (1) {
    printf("i am main thread -----\n");
    sleep(1);
  }

  return 0;
}
