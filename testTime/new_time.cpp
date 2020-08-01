#include <iostream>
#include <ctime>
using namespace std;

int main() {
    //两种获取当前时间完全相同
    time_t cur1;
    time(&cur1);
    cout << "time_t cur1 : " << cur1 << endl;

    time_t cur2 = time(NULL);
    cout << "time_t cur2 : " << cur2 << endl;
    
    cout << "两种获取当前时间完全相同" << endl;

    /*
     struct tm {
         int tm_sec;     // 秒：取值区间为[0,59] 
         int tm_min;     // 分：取值区间为[0,59] 
         int tm_hour;    // 时：取值区间为[0,23] 
         int tm_mday;    // 日期：一个月中的日期：取值区间为[1,31]
         int tm_mon;     // 月份：（从一月开始，0代表一月），取值区间为[0,11]
         int tm_year;    // 年份：其值等于实际年份减去1900
         int tm_wday;    // 星期：取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 
         int tm_yday;    // 从每年的1月1日开始的天数：取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 
         int tm_isdst;   // 夏令时标识符，该字段意义不大，我们不用夏令时。
     }; 
    */
    struct tm* stm_ptr;
    stm_ptr = localtime(&cur1);
    cout << "tm.tm_year:" << stm_ptr->tm_year + 1900 << endl;
    cout << "tm.tm_mon:" << stm_ptr->tm_mon + 1<< endl;
    cout << "tm.tm_mday:" << stm_ptr->tm_mday << endl;
    cout << "tm.tm_hour:" << stm_ptr->tm_hour << endl;
    cout << "tm.tm_min:" << stm_ptr->tm_min << endl;
    cout << "tm.tm_sec:" << stm_ptr->tm_sec << endl;

    char buf[1024];
    strftime(buf, 1024, "%Y-%m-%d %H:%M:%S", stm_ptr);
    cout << "经过使用strftime函数格式化后的日期为：" << buf << endl;

    return 0;
}
