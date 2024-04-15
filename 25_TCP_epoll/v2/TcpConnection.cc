#include "TcpConnection.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd)
    : _sockIO(fd), _sock(fd), _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()) {}

TcpConnection::~TcpConnection() {}

void TcpConnection::send(const string &msg) {
  _sockIO.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive() {
  char buf[65535] = {0};
  _sockIO.readLine(buf, sizeof(buf));
  return string(buf);
}

bool TcpConnection::isClosed() const {
  char buf[10] = {0};
  int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);
  return (0 == ret);
}

string TcpConnection::toString() {
  ostringstream oss;
  oss << _localAddr.ip() << ":" << _localAddr.port() << "---->"
      << _peerAddr.ip() << ":" << _peerAddr.port();
  return oss.str();
}

InetAddress TcpConnection::getLocalAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr);
  int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    perror("getsockname");
  }
  return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr);
  //获取对端地址的函数getpeername
  int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    perror("getpeername");
  }

  return InetAddress(addr);
}

void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback &cb) {
  _onNewConnectionCb = cb;
}
void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb) {
  _onMessageCb = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb) {
  _onCloseCb = cb;
}

void TcpConnection::handleNewConnectionCallback() {
  if (_onNewConnectionCb) {
    _onNewConnectionCb(shared_from_this());
  } else {
    cout << "_onNewConnectionCb ==nullptr" << endl;
  }
}

void TcpConnection::handleMessageCallback() {
  if (_onMessageCb) {
    _onMessageCb(shared_from_this());
  } else {
    cout << "_onMessageCb == nullptr" << endl;
  }
}

void TcpConnection::handleCloseCallback() {
  if (_onCloseCb) {
    _onCloseCb(shared_from_this());
  } else {
    cout << "_onCloseCb == nullptr" << endl;
  }
}
