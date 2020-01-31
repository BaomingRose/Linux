//体会文件描述符分配规则
//通过文件描述符分配规则体会重定向
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main() {
  close(1);

  int fd = open("./test.txt", O_CREAT | O_RDWR, 0664);
  if (fd < 0) {
    perror("open error");
    return -1;
  }

  printf("fd:%d\n", fd);
  close(fd);

  return 0;
}
