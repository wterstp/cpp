// header_template.h
#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include "MutexLock.h"
#include <functional>
#include <map>
#include <memory>
#include <vector>

using std::function;
using std::map;
using std::shared_ptr;
using std::vector;
class Acceptor;
class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
using Functor = function<void()>;

class EventLoop {
public:
  EventLoop(Acceptor &acceptor);
  ~EventLoop();
  void loop();
  void unloop();

private:
  void waitEpollFd();
  void handleNewConnection();
  void handleConnection();
  void handleMessage(int fd);

  int createEpollFd();
  void addEpollReadFd(int fd);
  void delEpollReadFd(int fd);

private:
  void handleRead();
  void doPengdingFunctors();
  int createEventFd();

public:
  void wakeup();
  void runInLoop(Functor &&cb);

public:
  void setNewConnectionCallback(TcpConnectionCallback &&cb);
  void setMessageCallback(TcpConnectionCallback &&cb);
  void setCloseCallback(TcpConnectionCallback &&cb);

private:
  int _epfd;
  vector<struct epoll_event> _evtList;
  bool _isLooping;
  Acceptor &_acceptor;
  map<int, TcpConnectionPtr> _conns;

  TcpConnectionCallback _onNewConnectionCb;
  TcpConnectionCallback _onMessageCb;
  TcpConnectionCallback _onCloseCb;

  int _evtfd;                //用于通信的文件描述符
  vector<Functor> _pendings; //待执行的"任务"
  MutexLock _mutex;          //互斥访问共享资源vector
};

#endif /* EVENTLOOP_H */
