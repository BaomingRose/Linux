#include <mysql/mysql.h>
#include <iostream>
#include <cstdio>
using namespace std;

//编译语句
//g++ insertMysql.cpp -o insertMysql `mysql_config --cflags --libs` -g
//注意：“`”这个符号是Esc下面的键，而不是单引号
//记得关闭数据库

int main() {
    //MYSQL 类型
    MYSQL conn;
    int res;

    //初始化
    mysql_init(&conn);

    //连接数据库
    if (mysql_real_connect(&conn, "localhost", "root", "123456", "tmp1", 0, NULL, CLIENT_FOUND_ROWS)) {
        cout << "数据库连接成功" << endl;

        //执行插入语句
        res = mysql_query(&conn, "insert into t1 values(4, 'wuyong') ");

        if (res) {
            cout << "执行插入失败" << endl;
        } else {
            cout << "执行插入成功" << endl;
        }
    } else {
        cout << "数据库连接失败" << endl;
        mysql_close(&conn);
        return -1;
    }
    mysql_close(&conn);

    return 0;
}
