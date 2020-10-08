#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>
using namespace std;

/*锁：
 * 互斥锁 mutex
 *      允许线程切换，只是线程不能往里走
 * 自旋锁 spinlock
 *      不允许线程切换
 *
 *  使用情况：mutex/spinlock
 *      时间短的用spinlock，时间长的/操作较复杂用mutex
 *
 *    原子操作，CAS-->Compare And Swap(原子操作的一种)
 *      指令集有这种指令才可以使用
 *
 *      if (a == b)
 *          a = c;
 *      比如单例模式，体系结构提供这种指令完成对比和赋值
 *
 */

pthread_mutex_t mutex;
pthread_spinlock_t spinlock;

//原子操作，将三条指令合成一条
int Inc(int* value, int add) {
    int old;

    __asm__ volatile(
        "lock; xaddl %2, %1;"
        : "=a" (old)
        : "m" (*value), "a"(add)
        : "cc", "memory"
    );

    return old;
}

void* thread_proc(void* arg) {
    int* p_count = (int*)arg;
    int i = 0;

    //执行加10w次
    while (i++ < 100000) {
#if 0
        ++*(p_count);
#elif 0
        //使用互斥锁
        pthread_mutex_lock(&mutex);
        ++*(p_count);
        pthread_mutex_unlock(&mutex);
#elif 0
        //使用自旋锁
        pthread_spin_lock(&spinlock);
        ++*(p_count);
        pthread_spin_unlock(&spinlock);
#else
        //原子操作，将++count为三条指令，先从内存拉到寄存器，将寄存器++，然后将寄存器送到内存
        Inc(p_count, 1);
#endif
        usleep(1);
    }

    return NULL;
}


int main() {
    int count = 0;

    pthread_mutex_init(&mutex, NULL);
    //第二个参数表示进程间共享
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED);

    pthread_t pid[10] = {0};
    for (int i = 0; i < 10; ++i) {
        pthread_create(&pid[i], NULL, thread_proc, &count);
    }

    for (int i = 0; i < 100; ++i) {
        printf("count ---> %d\n", count);
        sleep(1);
    }

    //等待其他线程
    getchar();

    return 0;
}
