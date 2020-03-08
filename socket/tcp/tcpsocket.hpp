#include <iostream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using std::cerr;
using std::string;

#define CHECK_RET(q) if((q)==false){return -1;}

//封装一个tcpsocket类，向外提供简单的套接字接口
//1.创建套接字
//2.为套接字绑定地址信息
//3.开始监听
//4.向服务端发起连接请求
//5.服务端获取新建连接
//6.发送数据
//7.接收数据
//8.关闭套接字

class TcpSocket {
  private:
    int _sockfd;

    int strToint(const string& str) {
      int ret;
      std::stringstream tmp;
      tmp << str;
      tmp >> ret;
      return ret;
    }

  public:
    void SetFd(const int fd) {
      _sockfd = fd;
    }

    int GetFd() {
      return _sockfd;
    }

    bool Socket() {
      _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if (_sockfd < 0) {
        cerr << "socket error\n";
        return false;
      }
      return true;
    }

    bool Bind(const string& ip, const string& port) {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(strToint(port));
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if (ret < 0 ) {
        cerr << "bind error\n";
        return false;
      }
      return true;
    }

    //服务端开始监听
    //backlog最大并发连接数，给的不是很高
    bool Listen(const int backlog = 5) {
      //int listen(int socket, int backlog);
      //开始监听：通知操作系统，可以接收客户端的连接请求了, 并且完成三次握手建立连接过程
      //tcp的面向连接，有一个三次握手建立连接过程
      //listen告诉操作系统可以替我们来完成三次握手建立连接过程
      //backlog:客户端最大并发连接数(同一时间最多接收多少个客户端新的连接请求)
      int ret = listen(_sockfd, backlog);
      if (ret < 0) {
        cerr << "listen error\n";
        return false;
      }
      return true;
    }

    //客户端发起连接请求
    bool Connect(const string& srv_ip, const string& srvport) {
      //int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
      //addr : 服务端地址信息
      //addrlen：地址信息长度
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(strToint(srvport));
      addr.sin_addr.s_addr = inet_addr(srv_ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
      if (ret < 0) {
        cerr << "connect error\n";
      }
      return true;
    }

    //服务端获取新建连接
    bool Accept(TcpSocket& clisock, string* ip = NULL, uint16_t* port = NULL) {
      //int accept(int socket, struct sockaddr* address,socklen_t* address_len);
      //socket：监听套接字描述符
      //address ：客户端地址信息
      //addr_len：地址信息长度
      //返回值：返回新建连接的socket描述符--用于与客户端进行单独数据通信
      struct sockaddr_in cliaddr;
      socklen_t len = sizeof(struct sockaddr_in);
      int newfd = accept(_sockfd, (struct sockaddr*)&cliaddr, &len);
      if (newfd < 0) {
        cerr << "accept error\n";
        return false;
      }
      clisock.SetFd(newfd);
      if (ip != NULL) {
        *ip = inet_ntoa(cliaddr.sin_addr);
      }
      if (port != NULL) {
        *port = ntohs(cliaddr.sin_port);
      }
      return true;
    }

    bool Send(const string& data) {
      //ssize_t send(int sockfd, const void *buf, size_t len, int flags);
      //sockfd：套接字描述符（服务端是新建连接的socket描述符）
      //buf：要发送的数据
      //len：要发送的数据长度
      //flags： 0-默认阻塞发送
      //返回值：成功：返回实际发送的数据长度  失败：-1
      int ret = send(_sockfd, &data[0], data.size(), 0);
      if (ret < 0) {
        cerr << "send error\n";
        return false;
      }
      return true;
    }
    
    bool Recv(string& buf) {
      //ssize_t recv(int sockfd, void *buf, size_t len, int flags);
      //flags : 0 - 默认阻塞接收  MSG_PEEK：从缓冲区取数据，但是数据并不从缓冲区移除
      //返回值：>0 表示实际接收的数据长度 ==0 表示连接断开  <0 出现错误
      char tmp[4096];
      int ret = recv(_sockfd, tmp, 4096, 0);
      if (ret < 0) {
        cerr << "recv error \n";
        return false;
      } else if (ret == 0) {
        cerr << "connect shutdown\n";
        return false;
      }
      buf.assign(tmp, ret);
      return true;
    }

    bool Close() {
      close(_sockfd);
    }
};
