/*基于select实现一个并发tcp服务器*/
/*对select进行封装, 封装一个类实例化一个对象*/

#include "tcpsocket.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <sys/time.h>
#include <sys/select.h>

class Select {
public:
    Select() : _maxfd(-1) {
        //一定不能忘记初始化  栽坑了，导致select错误
        FD_ZERO(&_rfds);
    }

    bool Add(TcpSocket& sock) {
        int fd = sock.GetFd();
        FD_SET(fd, &_rfds);
        _maxfd = _maxfd > fd ? _maxfd :fd;
        return true;
    }

    bool Del(TcpSocket& sock) {
        int fd = sock.GetFd();
        FD_CLR(fd, &_rfds);
        //更新_maxfd
        for (int i = _maxfd; i >= 0; i--) {
            if (FD_ISSET(i, &_rfds)) {
                _maxfd = i;
                return true;
            }
        }
        _maxfd = -1;
        return true;
    }

    bool Wait(std::vector<TcpSocket>& list, int sec = 3) {
        struct timeval tv;
        tv.tv_sec = sec;
        tv.tv_usec = 0;
        int count;
        fd_set tmp_set = _rfds;   //每次定义新的集合进行监控
        //避免对象的监控集合被修改, 避免每次重新向集合中添加描述符
        count = select(_maxfd + 1, &tmp_set, NULL, NULL, &tv);
        if (count < 0) {
            std::cout << "select error\n";
            perror("select error");
            return false;
        } else if (count == 0) {
            std::cout << "wait timeout\n";
            return false;
        }
        for (int i = 0; i <= _maxfd; ++i) {
            if (FD_ISSET(i, &tmp_set)) {
                TcpSocket sock;
                sock.SetFd(i);
                list.push_back(sock);
            }
        }
        return true;
    }

private:
    fd_set _rfds;
    int _maxfd;
};


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "./tcp_select ip port\n";
        return -1;
    }
    std::string srv_ip = argv[1];
    std::string srv_port = argv[2];
    TcpSocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(srv_ip, srv_port));
    CHECK_RET(sock.Listen());

    //将监听描述符添加到位图
    Select s;
    s.Add(sock);

    while (1) {
        std::vector<TcpSocket> list;
        if (!s.Wait(list)) {
            continue;
        }
        for (auto cli : list) {
            if (cli.GetFd() == sock.GetFd()) {
                //当前就绪的是监听套接字，应该Accept
                if (!sock.Accept(cli)) {
                    continue;
                }
                s.Add(cli);
            } else {
                //普通通信套接字, 应该Recv
                std::string buf;
                if (!cli.Recv(buf)) {
                    s.Del(cli);
                    cli.Close();
                    continue;
                }
                std::cout << "client say:  {" << buf << "}\n";
                buf.clear();
                std::cin >> buf;
                if (!cli.Send(buf)) {
                    s.Del(cli);
                    cli.Close();
                    continue;
                }
            }
        }
    }
    sock.Close();

    return -1;
}
