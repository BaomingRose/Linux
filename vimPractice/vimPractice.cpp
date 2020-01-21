#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <map>

using namespace std;

void test1() {
  cout << "test1()" << endl;
}

void test2() {
  cout << "test2() " << endl;
}

void test3() {
  cout << "test3()" << endl;
}

void test4() {
  cout << "test4()" << endl;
}

void test5() {
  cout << "test5()" << endl;
}

void test6() {
  cout << "test6()" << endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();

  return 0;
}

/*
1.列选择:
  ctrl + v 进入可视化模式，然后进行勾选，然后I可以整列插入，或者x选择的都删除
2.可视化行号:
  （:进入底行模式）set nonu 取消行号    set nu  加行号
3.启动多个文件： 
  （:进入底行模式）vs [其他文件] 可以同时查看其他文件    
    改变光标左右切换 ctrl + ww
4.查找:
  （普通模式下）查找 / + 要查找的内容 回车查找第一个 n 代表查找下（next）
          ？ 向上搜索
5.替换:
  （:进入底行模式）替换 %s/ 要替换的 / 替换后的/g（是否替换行内所有出现的需要替换的单词）
          s/要替换的/替换后的/g
       （ 加%表示替换全文，不加表示替换光标所在行）
6.调整格式:
  沾代码的时候很乱 gg = G    改正格式
7.cw
   删除光标后1个词
   c [num]w
   删除num个词
*/
