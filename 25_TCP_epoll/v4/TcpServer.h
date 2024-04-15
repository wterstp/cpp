// header_template.h
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Acceptor.h"
#include "EventLoop.h"

class TcpServer {
public:
  TcpServer(const string &ip, unsigned short port);
  ~TcpServer();

  void start();
  void stop();
  void setAllCallback(TcpConnectionCallback &&cb1, TcpConnectionCallback &&cb2,
                      TcpConnectionCallback &&cb3);

private:
  Acceptor _acceptor;
  EventLoop _loop;
};

#endif /* TCPSERVER_H */
