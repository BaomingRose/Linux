#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using std::string;

#define CHECK_RET(q) if((q)==false){return -1;}

//封装UdpSocket类，实例化对象，向外提供简单的socket接口
//1.创建套接字
//2.为套接字绑定地址信息
//3.发送数据
//4.接收数据
//5.关闭套接字
class UdpSocket {
  private:
    int _sockfd;
  public:
    bool Socket() {
      //int socket(int domain, int type, int protocol); 
      _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if (_sockfd < 0) {
        std::cerr << "socket error\n";
        return false;
      }
      return true;
    }

    bool Bind(const std::string& ip, uint16_t port) {
      //int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;

      //uint16_t htons(uint16_t hostshort);
      //将主机字节序16位数据转换为网络字节序
      addr.sin_port = htons(port);

      //192.168.70.128  --> 0xc0 a8 46 80
      //addr.sin_addr.s_addr = htonl(0xc0a84680);
      //上面方法麻烦
      //将点分十进制字符串ip地址转换为网络字节序ip地址
      //in_addr_t inet_addr(const char *cp);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());

      int ret;
      socklen_t len = sizeof(struct sockaddr_in);
      ret = bind(_sockfd, (sockaddr*)&addr, len);
      if (ret < 0) {
        std::cerr << "bind error\n";
        return false;
      }

      return true;
    }

    bool Send(const std::string& data, const std::string& peer_ip, const uint16_t peer_port) {
      //ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(peer_port);
      addr.sin_addr.s_addr = inet_addr(peer_ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = sendto(_sockfd, &data[0], data.size(), 0, (sockaddr*)&addr, len);
      if (ret < 0) {
        std::cerr << "sendto error\n";
        return false;
      }
      return true;
    }

    bool Recv(std::string& buf, std::string& peer_ip, uint16_t& peer_port) {
      //ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
      //成功:返回实际接收的数据长度， 失败：返回-1
      struct sockaddr_in peer_addr;
      socklen_t len = sizeof(struct sockaddr_in);
      char tmp[4096] = { 0 };
      int ret = recvfrom(_sockfd, tmp, 4096, 0, (sockaddr*)&peer_addr, &len);
      if (ret < 0) {
        std::cerr << "recvfrom error\n";
        return false;
      }
      //从tmp拷贝ret字符到string buf中
      buf.assign(tmp, ret);
      
      ////将网络字节序ip地址转换为点分十进制字符串ip地址
      //char *inet_ntoa(struct in_addr in);
      peer_ip = inet_ntoa(peer_addr.sin_addr);
      //将网络字节序的16位数据转换为主机字节序数据
      //uint16_t ntohs(uint16_t netshort);
      peer_port = ntohs(peer_addr.sin_port);
      return true;
    }

    void Close() {
      close(_sockfd);
    }
};
