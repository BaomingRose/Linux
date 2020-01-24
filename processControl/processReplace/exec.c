#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[], char* env[]) {
  printf("leihoua!!!\n");
  //int execl( const char *path, const char *arg, ... );
  //使用path这个路径的程序，替换当前进程要运行的程序
  //后边arg以及...的都是这个程序的运行参数, 要以NULL结尾
  //execl("/usr/bin/ls", "ls", "-l", "-a", NULL);
  //execlp("ls", "ls", "-l", "-a", NULL);
  //execlp("pwd", "pwd", NULL);
  
  /*
  char* argv[32] = { NULL };
  argv[0] = "ls";
  argv[1] = "-l";
  argv[2] = "-a";
  //execv("/usr/bin/ls", argv);
  //execv("./test", argv);
  */
  
  //int execle( const char *path, const char *arg , ..., char * const envp[] );
  //这个单词最后的字母e表示需要自定义环境变量
  execle("./test", "test", "-l", NULL, env);
  
  printf("nihaoa~~~\n");

  return 0;
}
