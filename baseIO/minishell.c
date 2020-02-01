#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
  while (1) {
    printf("[root@localhost ~]$ ");
    fflush(stdout);   //刷新缓冲区，不然上面的打印不出来
    
    char tmp[1024] = { 0 };
    scanf("%[^\n]%*c", tmp);  //接收一行，从缓冲区丢掉换行符
    //printf("%s\n", tmp);

    //ls -l >> test.txt
    char* ptr = tmp;
    int redirect_flag = 0;
    char* redirect_file = NULL;
    while (*ptr != '\0') {
      if (*ptr == '>') {
        *ptr = '\0';
        ptr++;
        redirect_flag = 1;  //清空重定向
        if (*ptr == '>') {
          *ptr = '\0';
          ptr++;
          redirect_flag = 2;  //追加重定向
        }
        while (isspace(*ptr) && *ptr != '\0')
          ptr++;
        redirect_file = ptr;
        while (!isspace(*ptr) && *ptr != '\0')
          ptr++;
        *ptr = '\0';
      }
      ptr++;
    }
    
    //需要将整体字符串解析出：程序名称+参数
    ptr = tmp;
    int argc = 0;
    char* argv[32] = { NULL };
    while (*ptr != '\0') {
      if (!isspace(*ptr)) {
        //指针走到非空白字符处
        argv[argc] = ptr;
        argc++;
        while (!isspace(*ptr) && *ptr != '\0')
          ptr++;
        *ptr = '\0';
        ptr++;
        continue;
      }
      ptr++;
    }
    argv[argc] = NULL;
    /*
    int i;
    for (i = 0; i < argc; ++i) {
      printf("argv[%d] = [%s]\n", i, argv[i]);
    }
    */

    //判断当前命令是否为内建命令
    if (!strcmp(argv[0], "cd")) {
      chdir(argv[1]);     //改变当前工作路径
      continue;
    }

    int pid = fork();
    if (pid == 0) {
      //ls浏览目录，实际是将目录内容写入到标准输出文件实现打印效果
      //标准输出重定向要做的就是把要写入到标准输出的数据不打印了而是写入到新文件中
      int fd = 1;
      if (redirect_flag == 1) {
        fd = open(redirect_file, O_CREAT | O_WRONLY | O_TRUNC, 0664);
      } else if (redirect_flag == 2) {
        fd = open(redirect_file, O_CREAT | O_WRONLY | O_APPEND, 0664);
      }
      //标准输出重定向，将原本要写入标准输出1的数据写入到新文件中
      dup2(fd, 1);

      execvp(argv[0], argv);
      //若子进程程序替换失败，则直接退出，因为终端不需要多个shell
      exit(0);
    }
    //等待子进程退出，避免僵尸进程
    wait(NULL);
  }

  return 0;
}
