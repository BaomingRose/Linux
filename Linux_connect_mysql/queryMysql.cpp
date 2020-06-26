#include <iostream>
#include <cstdio>
#include <mysql/mysql.h>
using namespace std;

//这里所用的指针（包括结果集, 列集…）…都没有自己开空间，自己需要拿指针来接收返回的空间即可
//编译语句：g++ queryMysql.cpp -o queryMysql `mysql_config --cflags --libs` -g

int main() {
    int res;
    MYSQL conn;
    mysql_init(&conn);

    if (mysql_real_connect(&conn, "localhost", "root", "123456", "tmp1", 0, NULL, CLIENT_FOUND_ROWS)) {
        cout << "数据库连接成功!" << endl;

        res = mysql_query(&conn, "select * from t2");
        if (res) {
            cout << "执行查询失败!" << endl;
        } else {
            //获取查询结果集合
            MYSQL_RES* res_ptr = mysql_store_result(&conn);

            if (res_ptr) {
                //查询结果的列数和行数
                int col = mysql_num_fields(res_ptr);
                int row = mysql_num_rows(res_ptr);
                printf("查询到 %d 行 \n", row + 1);

                MYSQL_FIELD* field;

                //打印列名
                while (field = mysql_fetch_field(res_ptr)) {
                    printf("%s\t", field->name);
                }
                cout << endl;

                //打印数据
                for (int i = 0; i < row; ++i) {
                    //获取查询结果的下一行
                    MYSQL_ROW result_row = mysql_fetch_row(res_ptr);

                    for (int j = 0; j < col; ++j) {
                        cout << result_row[j] << "\t";
                    }
                    cout << endl;
                }
            }
        }

    } else {
        cout << "数据库连接失败!" << endl;
        mysql_close(&conn);
        return -1;
    }
    mysql_close(&conn);

    return 0;
}

