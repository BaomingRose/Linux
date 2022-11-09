#ifdef USE_DYNAMICLIB   // 使用动态链接的方式打开
#include <iostream>
#include "../include/calc.h"
using namespace std;

int main() {
    cout << add_cal(1, 2) << endl;

    return 0;
}
#endif


#ifdef USE_DLOPEN   //使用dlopen打开
#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

typedef int(*Add)(int, int);
const char* lib_path = "/lib64/libcalc.so";

int main() {
    void* dlptr = dlopen(lib_path, RTLD_NOW);
    if (NULL == dlptr) {
        perror(dlerror());
        return -1;
    }
    cout << "dlptr:" << dlptr << endl;

    Add add_cal = (Add)dlsym(dlptr, "_Z7add_calii");  // 使用nm libXX.so 查看函数对应的符号，直接使用add_cal会找不到
    if (NULL == add_cal) {
        perror(dlerror());
        dlclose(dlptr);
        return -1;
    }

    cout << "add_cal ptr:" << add_cal << endl;
    cout << add_cal(10, 100) << endl;
}
#endif
