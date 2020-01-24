#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//这个程序功能：打印传进函数的参数列表和系统环境变量
//为了让exec文件程序替换，测试带e与不带e的区别
//带e需要自定义环境变量
//不带e默认使用系统环境变量

int main(int argc, char* argv[], char* env[]) {
  int i;
  for (i = 0; argv[i] != NULL; ++i) {
    printf("argv[%d] = [%s]\n", i, argv[i]);
  }
  for (i = 0; env[i] != NULL; ++i) {
    printf("env[%d] = [%s]\n", i, env[i]);
  }

  return 0;
}
