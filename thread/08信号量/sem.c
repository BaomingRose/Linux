#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

//体会posix标准信号量的基本操作

int ticket = 100;
sem_t sem;

void* thr_start(void* arg) {
  while (1) {
    //int sem_wait(sem_t *sem);
    //对计数进行判断，
    //是否>0；若大于0则函数立即正确放回，返回前计数-1，程序流程继续向下可以操作临界资源
    //若小于等于0，则认为当前不能对临界资源进行操作，则线程等待
    //int sem_trywait(sem_t *sem);
    //若当前计数<=0，直接报错返回
    //int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
    //若当前计数<=0，等待时间超时后报错返回 -----  errno==ETIMEDOUT 
    sem_wait(&sem);
    if (ticket > 0) {
      printf("thread[%d]:get a ticket:%d\n", *(int*)arg, ticket);
      ticket--;
    } else {
      sem_post(&sem);
      pthread_exit(NULL);
    }
    //int sem_post(sem_t *sem);
    //计数+1，促使其他线程条件满足，然后唤醒所有等待线程
    sem_post(&sem);
  }

  return NULL;
}

int main() {
  int i;
  pthread_t tid[4];
  //int sem_init(sem_t *sem, int pshared, unsigned int value);
  sem_init(&sem, 0, 1);

  int arr[] = { 0, 1, 2, 3 };
  for (i = 0; i < 4; ++i) {
    int ret = pthread_create(tid + i, NULL, thr_start, arr + i);
    if (ret != 0) {
      printf("pthread create error\n");
      return -1;
    }
  }
  for (i = 0; i < 4; ++i) {
    pthread_join(tid[i], NULL);
  }

  //int sem_destroy(sem_t *sem);
  sem_destroy(&sem);

  return 0;
}
