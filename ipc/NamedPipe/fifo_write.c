#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

//实现一个简单命名管道通信
//体会命名管道的一些特性
//int mkfifo(const char *pathname, mode_t mode);
//pathname：管道文件名称
//mode：管道文件权限
//返回值：0   失败：-1

int main() {
  char* fifo = "./test.fifo";
  int ret = mkfifo(fifo, 0664);
  if (ret < 0) {
    //如果文件存在，并不让它退出
    if (errno != EEXIST) {
      perror("mkfifo error");
      return -1;
    }
  }

  //打开特性：
  //若文件没有被已经以读的方式打开，则以O_WRONLY打开时会阻塞
  //若文件没有被已经以写的方式打开，则以O_RDONLY打开时会阻塞
  printf("open fifo-----\n");
  int fd = open(fifo, O_WRONLY);
  if (fd < 0) {
    perror("open error");
    return -1;
  }
  printf("fifo:%s open success!\n", fifo);

  while (1) {
    char buf[1024] = { 0 };
    printf("i say:");
    fflush(stdout);
    scanf("%s", buf);
    write(fd, buf, strlen(buf));
  }
  close(fd);

  return 0;
}

