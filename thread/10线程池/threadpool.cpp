#include <iostream>
#include <queue>
#include <pthread.h>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#define THR_MAX 5

typedef void (*handler_t)(int data);
class Task {
  private:
    int _data;
    handler_t _handler;
  public:
    void setdata(int data, handler_t func) {
      _data = data;
      _handler = func;
    }
    void run() {
      _handler(_data);
    }
};

class ThreadPool {
  private:
    int th_max;
    std::queue<Task> _task_queue;
    pthread_mutex_t mutex;
    pthread_cond_t pro_cond;
    pthread_cond_t con_cond;
    int _cur_thr; //当前线程数
    bool _quit_flag;  //线程池中线程退出标志
    
  private:
    void QueueBlock() {
      pthread_mutex_lock(&mutex);
    }
    void QueueUnBlock() {
      pthread_mutex_unlock(&mutex);
    }
    void ConsumerWait() {
      //线程陷入等待之前先判断一下用户是否要销毁线程池
      if (_quit_flag == true) {
        _cur_thr--;
        pthread_mutex_unlock(&mutex);
        printf("thread:%p exit\n", pthread_self());
        pthread_exit(NULL);
      }
      pthread_cond_wait(&con_cond, &mutex);
    }
    void ConsumerWakeUp() {
      pthread_cond_signal(&con_cond);
    }
    void ConsumerWakeUpAll() {
      pthread_cond_broadcast(&con_cond);
    }
    bool QueueIsEmpty() {
      return _task_queue.empty();
    }

  public:
    ThreadPool(int max = THR_MAX) : th_max(max), _cur_thr(max) {
      _quit_flag = false;
      pthread_mutex_init(&mutex, NULL);
      pthread_cond_init(&con_cond, NULL);
      pthread_cond_init(&pro_cond, NULL);
    }

    ~ThreadPool() {
      pthread_mutex_destroy(&mutex);
      pthread_cond_destroy(&con_cond);
      pthread_cond_destroy(&pro_cond);
    }

    //不能放入构造函数的原因：构造函数没有返回值判断
    bool PoolInit() {
      int ret;
      pthread_t tid;
      for (int i = 0; i < th_max; ++i) {
        //将this做static函数的参数,调用this的成员
        ret = pthread_create(&tid, NULL, thr_start, this);
        if (ret != 0) {
          std::cout << "pthread create error\n";
          return false;
        }
        pthread_detach(tid);
      }
      return true;
    }

    bool TaskPush(Task &t) {
      pthread_mutex_lock(&mutex);
      _task_queue.push(t);
      pthread_cond_signal(&con_cond);
      pthread_mutex_unlock(&mutex);

      return true;
    }

    bool TaskPop(Task &t) {
      t = _task_queue.front();
      _task_queue.pop();

      return true;
    }

    //static原因：如果不是static，这个函数还有一个默认参数this，这个函数就不能做创建进程函数的参数了
    static void* thr_start(void* arg) {
      ThreadPool* pool = (ThreadPool*)arg;

      //这层循环不可以忘记，要循环处理
      while (1) {
        pthread_mutex_lock(&pool->mutex);
        while (pool->_task_queue.empty()) {
          pool->ConsumerWait();
          //pthread_cond_wait(&pool->con_cond, &pool->mutex);
        }
        Task t;
        pool->TaskPop(t);
        pthread_mutex_unlock(&pool->mutex);

        //解锁之后运行
        t.run();
      }
      return NULL;
    }

    //为了避免主函数退出，导致所有线程退出, 让所有线程处理完再退出
    void PoolQuit() {
      QueueBlock();
      if (_quit_flag == false) {
        _quit_flag = true;
      }
      QueueUnBlock();
      while (_cur_thr > 0) {
        ConsumerWakeUpAll();
        usleep(1000);
      }
    }
};

void thr_handle(int data) {
  srand(time(NULL));
  int sec = rand() % 5;
  //std::cout << "thr: " << pthread_self() << "get data:" << data << "sleep" << sec << "sec" <<\
    std::endl;
  printf("thr:%p--get data:%d--sleep:%d sec\n", pthread_self(), data, sec);
  sleep(sec);
}

int main() {
  ThreadPool pool;
  
  pool.PoolInit();
  for (int i = 0; i < 10; ++i) {
    Task* task = new Task();
    task->setdata(i, thr_handle);
    pool.TaskPush(*task);
  }

  pool.PoolQuit();
  //while (1) {

  //}

  return 0;
}
