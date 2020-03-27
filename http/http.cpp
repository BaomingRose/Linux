#include "tcpsocket.hpp"
using namespace std;

//实现一个简单的http服务器

int main(int argc, char* argv[]) {
    TcpSocket sock;
    CHECK_RET(sock.Socket());
    string str = "0.0.0.0";
    CHECK_RET(sock.Bind(str,"9000"));
    CHECK_RET(sock.Listen());
    while (1) {
        TcpSocket clisock;
        if (sock.Accept(clisock) == false) {
            continue;
        }
        string buf;
        clisock.Recv(buf);
        cout << "req:[" << buf << "]\n";

        string body = "<html><body><h1>hello world</h1></body></html>";
        body += "<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"";
        //string first = "HTTP/1.1 200 OK\r\n";
        string first = "HTTP/1.1 302 OK\r\n";
        stringstream ss;
        ss << "Content-Length: " << body.size() << "\r\n";
        ss << "Location:http://www.taobao.com/\r\n";
        string head  = ss.str();
        string blank = "\r\n";

        clisock.Send(first);
        clisock.Send(head);
        clisock.Send(blank);
        clisock.Send(body);

        clisock.Close();
    }

    return 0;
}
