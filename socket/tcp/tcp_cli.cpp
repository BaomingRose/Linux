#include "tcpsocket.hpp"
#include <cstdio>
#include <signal.h>
using std::cout;
using std::endl;
using std::cin;

//1.创建套接字
//2.为套接字绑定地址信息(不推荐用户主动绑定) //因为只能绑定一个固定的地址，客户端只能启动一个
//3.向服务端连接请求
//4.发送数据
//5.接受数据
//6.关闭套接字

void sigcb(int signo) {
  printf("recv a sigo SIGPIPE---- Connect shutdown\n");
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cerr << "参数信息有误！" << endl << "./tcp_cli ip port\n";
    return -1;
  }
  signal(SIGPIPE, sigcb);
  TcpSocket sock;
  //1.创建套接字
  CHECK_RET(sock.Socket());
  //2.为套接字绑定地址信息(不推荐用户主动绑定) 
  //3.向服务端连接请求
  CHECK_RET(sock.Connect(argv[1], argv[2]));
  while (1) {
    //4.发送数据
    string buf;
    cout << "client say : ";
    fflush(stdout);
    cin >> buf;
    sock.Send(buf);
    //5.接受数据
    buf.clear();
    sock.Recv(buf);
    cout << "sever say : " << buf << endl;
  }
  //6.关闭套接字
  sock.Close();

  return 0;
}
