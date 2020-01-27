#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//环境变量(environment variables)一般是指在操作系统中用来指定操作系统运行环境的一些参数
//环境变量通常具有某些特殊用途，还有在系统当中通常具有全局特性

#if 0
//命令行第三个参数获取环境变量
int main(int argc, char* argv[], char* env[]) {
  int i = 0;
  for (; env[i]; i++) {
    printf("%s\n", env[i]);
  }

  return 0;
}
#endif

#if 0
//通过第三方变量environ获取环境变量
int main() {
  extern char** environ;
  int i = 0;
  for (; environ[i]; i++) {
    printf("%s\n", environ[i]);
  }

  return 0;
}
#endif

//getenv获取环境变量
int main() {
  char* env = getenv("PATH");
  if (env) {
    printf("%s\n", env);
  }

  return 0;
}
