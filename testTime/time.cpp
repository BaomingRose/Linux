#include <iostream>
#include <sys/time.h>
#include <cstdio>
using namespace std;

int main() {
    struct timeval t;
    //返还一个时间戳
    gettimeofday(&t, NULL);
    cout << t.tv_sec << endl;

    time_t Systime;
    //获取秒级时间戳
    time(&Systime);
    //将时间戳转换为时间
    struct tm* ST = localtime(&Systime);
    char Timenow[23] = { 0 };
    snprintf(Timenow, sizeof(Timenow) - 1, "%04d-%02d-%02d %02d:%02d:%02d", ST->tm_year + 1900, ST->tm_mon + 1, ST->tm_mday, ST->tm_hour, ST->tm_min, ST->tm_sec);

    printf("%s\n", Timenow);

    return 0;
}
