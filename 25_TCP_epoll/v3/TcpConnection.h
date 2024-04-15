// header_template.h
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <functional>
#include <memory>

using std::function;
using std::shared_ptr;

class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {

public:
  explicit TcpConnection(int fd);
  ~TcpConnection();
  void send(const string &msg);
  string receive();
  string toString();
  bool isClosed() const;

  void setNewConnectionCallback(const TcpConnectionCallback &cb);
  void setMessageCallback(const TcpConnectionCallback &cb);
  void setCloseCallback(const TcpConnectionCallback &cb);

  void handleNewConnectionCallback();
  void handleMessageCallback();
  void handleCloseCallback();

private:
  InetAddress getLocalAddr();
  InetAddress getPeerAddr();

private:
  SocketIO _sockIO;
  Socket _sock;
  InetAddress _localAddr;
  InetAddress _peerAddr;

  TcpConnectionCallback _onNewConnectionCb; //连接建立
  TcpConnectionCallback _onMessageCb;       //消息到达
  TcpConnectionCallback _onCloseCb;         //连接断开
};

#endif /* TCPCONNECTION_H */
