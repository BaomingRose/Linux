#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * write 是系统调用接口，没有缓冲区，通过fd直接写入文件
 * FILE 结构体里面封装了两个（读/写）缓冲区，遇到\n或者退出才会刷新缓冲区
*/

#if 0
int main() {
  printf("printf\n");
  fprintf(stdout, "fprintf\n");
  fwrite("fwrite\n", 6, 1, stdout);
  write(1, "write\n", 5);
  sleep(3);

  return 0;
}

/*运行结果
printf
fprintf
writefwrite
*/
#endif

int main() {
  printf("printf");
  fprintf(stdout, "fprintf");
  fwrite("fwrite", 6, 1, stdout);
  write(1, "write", 5);
  sleep(3);

  return 0;
}

/*运行结果:
 writeprintffprintffwrite
*/
