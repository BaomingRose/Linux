#include "tcpsocket.hpp"
#include <cstdio>
using std::cout;
using std::endl;
using std::cin;

//1.创建套接字
//2.为套接字绑定地址信息
//3.开始监听
//4.获取已完成连接socket
//5.通过获取的新建socket与客户端进行通信--接收数据
//6.回复（发送）数据
//7.关闭套接字

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cerr << "运行参数不正确" << endl << "./tcp_srv ip port\n";
    return -1;
  }
  //监听套接字
  TcpSocket lst_sock;
  //1.创建套接字
  CHECK_RET(lst_sock.Socket());
  //2.为套接字绑定地址信息
  CHECK_RET(lst_sock.Bind(argv[1], argv[2]));
  //3.开始监听
  CHECK_RET(lst_sock.Listen());
  while (1) {
    //4.获取已完成连接socket
    TcpSocket clisock;
    bool ret = lst_sock.Accept(clisock);
    if (ret == false) {
      continue;
    }
    //5.通过获取的新建socket与客户端进行通信--接收数据
    string buf;
    ret =  clisock.Recv(buf);
    //如果客户端断开，关闭套接字
    if (ret == false) {
      clisock.Close();
      continue;
    }
    cout << "client say : " << buf << endl;
    //6.回复（发送）数据
    buf.clear();
    cout << "sever say : " ;
    //没有'\n', 需要刷新缓冲区
    //cout.sync_with_stdio(true);
    fflush(stdout);
    cin >> buf;
    clisock.Send(buf);
  }
  //7.关闭套接字
  lst_sock.Close();

  return 0;
}
