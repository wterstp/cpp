// header_template.h
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
class TcpConnection {
public:
  explicit TcpConnection(int fd);
  ~TcpConnection();
  void send(const string &msg);
  string receive();
  string toString();

private:
  InetAddress getLocalAddr();
  InetAddress getPeerAddr();

private:
  SocketIO _sockIO;
  Socket _sock;
  InetAddress _localAddr;
  InetAddress _peerAddr;
};

#endif /* TCPCONNECTION_H */
