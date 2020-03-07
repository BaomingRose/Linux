#include "udpsocket.hpp"
#include <sstream>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "./udp_srv 192.168.70.128  9000\n";
    return -1;
  }
  string ip = argv[1];
  uint16_t port; 
  std::stringstream tmp;
  tmp << argv[2];
  tmp >> port;

  UdpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Bind(ip, port));

  while (1) {
    string buf;
    string peer_ip;
    uint16_t peer_port;
    sock.Recv(buf, peer_ip, peer_port);
    std::cout << "client - [" << peer_ip << ":" << peer_port << "] say:" << buf << std::endl;

    buf.clear();
    std::cin >> buf;
    sock.Send(buf, peer_ip, peer_port);
  }
  sock.Close();

  return 0;
}
