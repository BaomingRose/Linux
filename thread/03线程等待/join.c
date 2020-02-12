#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>


void* thr_entry(void* arg) {
  sleep(3);
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

  void* retval;
  //int pthread_join(pthread_t thread, void **retval);
  ret = pthread_join(tid, &retval);
  if (ret == EINVAL) {
    printf("thread is not joinable\n");
  }
  printf("tid: %p ------- retval:%s \n", tid, retval);   //无符号长整型%lu
  while (1) {
    printf("i am main thread -----\n");
    sleep(1);
  }

  return 0;
}
