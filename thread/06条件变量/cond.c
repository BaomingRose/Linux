#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//条件变量的基本使用
//顾客和厨师做面为例
//顾客吃面，前提是有面，没有面则等待
//厨师做好面后唤醒吃面的人

#if 0   //一个厨师和一个顾客
int _hava_noodle = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

void* eat_noodle(void* arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    if (_hava_noodle == 0) {
      //没有面，就不能吃面
      //continue;
      //阻塞等待
      //int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
      //用户在判断条件不满足的情况下提供等待功能
      //wait实现了三步操作：
      //1.解锁
      //2.休眠
      //3.被唤醒后加锁
      //其中解锁和休眠操作必须是原子操作
      pthread_cond_wait(&cond, &mutex);

      /*int pthread_cond_timedwait(pthread_cond_t *restrict cond,
        pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime); */
      //限时等待，等待超时后报错返回
    }
    //noodle=1,有面
    printf("eat noodle, delicious\n");
    _hava_noodle = 0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void* cook_noodle(void* arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    if (_hava_noodle == 1) {
      //有面，没人吃，不能继续做
      //continue;
      pthread_cond_wait(&cond, &mutex);
    }
    printf("cook_noodle, come on\n");
    _hava_noodle = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main() {
  pthread_t tid1, tid2;

  pthread_mutex_init(&mutex, NULL);
  //int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
  pthread_cond_init(&cond, NULL);

  int ret = pthread_create(&tid1, NULL, eat_noodle, NULL);
  if (ret != 0) {
    printf("thread create error\n");
    return -1;
  }

  ret = pthread_create(&tid2, NULL, cook_noodle, NULL);
  if (ret != 0) {
    printf("thread create error\n");
    return -1;
  }

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_mutex_destroy(&mutex);
  //int pthread_cond_destroy(pthread_cond_t *cond);
  pthread_cond_destroy(&cond);

  return 0;
}
#endif

//多个厨师和多个顾客, 一个碗
int _hava_noodle = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_eat;
pthread_cond_t cond_cook;

void* eat_noodle(void* arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    while (_hava_noodle == 0) {
      //没有面，就不能吃面
      pthread_cond_wait(&cond_eat, &mutex);
    }
    //noodle=1,有面
    sleep(1);
    printf("顾客%d：eat noodle, delicious\n", *(int*)arg);
    _hava_noodle = 0;
    pthread_cond_signal(&cond_cook);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void* cook_noodle(void* arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    while (_hava_noodle == 1) {
      //有面，没人吃，不能继续做
      //continue;
      pthread_cond_wait(&cond_cook, &mutex);
    }
    sleep(1);
    printf("厨师%d：cook_noodle, come on\n", *(int*)arg);
    _hava_noodle = 1;
    pthread_cond_signal(&cond_eat);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
/*  
 *条件变量的条件判断应该是-个循环判断:
原因:多个顾客线程若被唤醒，只有一个顾客可以加锁，其他的顾客线程将阻塞在加锁上(而不是条件变量的等待);
如果是if判断则可能顾客都开始吃面; 
第一个加锁的顾客开始吃面，吃完面后进行解锁，这时候，获取到锁的线程可能是多个顾客线程，因为再次判断有没有面, 因此直接吃面,
但是面已经被第一个顾客线程吃掉了， 因此逻辑错误。应该加锁之后重新再次判断是否有面

 *不同的角色应该等待在不同的条件变量上:
在有多个厨师线程和顾客线程的时候，若是顾客和厨师线程都等待在同-个条件变量的等待队列中，会导致厨师做了-碗面， 本应该唤醒顾客线程吃面,
但是这时候有可能唤醒的是-个厨师线程，而厨师线程因为循环判断有没有面，因为已经有面而陷入等待(而顾客线程因为没有被唤醒而无法吃面)
*/

int main() {
  pthread_t tid1, tid2;

  pthread_mutex_init(&mutex, NULL);
  //int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
  pthread_cond_init(&cond_eat,  NULL);
  pthread_cond_init(&cond_cook, NULL);

  int arr[] = { 0, 1, 2, 3 };
  for (int  i = 0; i < 4; ++i) {
    int ret = pthread_create(&tid1, NULL, eat_noodle, arr + i);
    if (ret != 0) {
      printf("thread create error\n");
      return -1;
    }
  }

  for (int i  = 0; i < 4; ++i) {
    int ret = pthread_create(&tid2, NULL, cook_noodle, arr + i);
    if (ret != 0) {
      printf("thread create error\n");
      return -1;
    }
  }

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_mutex_destroy(&mutex);
  //int pthread_cond_destroy(pthread_cond_t *cond);
  pthread_cond_destroy(&cond_eat);
  pthread_cond_destroy(&cond_cook);

  return 0;
}
