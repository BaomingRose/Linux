#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#if 1
int main() {
  //printf将数据并没有直接写入文件，而是写入缓冲区，直到\n或者程序退出才会刷新缓冲区将数据写入文件(显示器文件)
  printf("------------------");     //睡眠3秒，等待程序退出刷新缓冲区
  //printf("------------------\n"); //遇见\n刷新缓冲区，将数据写入文件
  sleep(3);

  //return 0; //只有在main函数中return是退出进程  //程序退出刷新缓冲区
  //exit(1);  //任意位置调用都是退出进程    //库函数接口  //程序退出刷新缓冲区
  _exit(1);   //系统调用接口  //没有刷新缓冲区  
}
#endif
