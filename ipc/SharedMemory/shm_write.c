#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

//演示共享内存的基本操作流程
#define IPC_KEY 0x12345678

int main() {
  //1.物理开辟一块内存空间
  //int shmget(key_t key, size_t size, int shmflg);
  int shmid = shmget(IPC_KEY, 32, IPC_CREAT | 0664);
  if (shmid < 0) {
    perror("shmget error");
    return -1;
  }

  //2.将共享内存通过页表映射到进程的虚拟地址空间中
  //void *shmat(int shmid, const void *shmaddr, int shmflg);
  void* shm_start = shmat(shmid, NULL, 0);
  if (shm_start == (void*)(-1)) {
    perror("shmat error");
    return -1;
  }

  //3.读取这块共享内存
  int i = 0;
  while (1) {
    sprintf(shm_start, "%s-%d", "hello world", i++);
    sleep(1);
  }

  //4.解除映射关系
  shmdt(shm_start);

  //5.删除共享内存
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
