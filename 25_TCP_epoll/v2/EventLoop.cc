#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpollFd()), _evtList(1024), _isLooping(false),
      _acceptor(acceptor) {
  int listenfd = _acceptor.fd();
  addEpollReadFd(listenfd);
}

EventLoop::~EventLoop() { close(_epfd); }

void EventLoop::loop() {
  _isLooping = true;
  while (_isLooping) {
    waitEpollFd();
  }
}

void EventLoop::unloop() { _isLooping = false; }
void EventLoop::waitEpollFd() {
  int nready = 0;
  do {
    nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 3000);
  } while (-1 == nready && errno == EINTR);

  if (-1 == nready) {
    cerr << "-1 == nready" << endl;
    return;
  } else if (0 == nready) {
    cout << ">>epoll_wait timeout" << endl;
  } else {
    //可以判断一下，文件描述符是不是已经达到了1024
    //如果达到1024就需要进行扩容
    if (nready == (int)_evtList.size()) {
      _evtList.reserve(2 * nready);
    }

    for (int idx = 0; idx < nready; ++idx) {
      int fd = _evtList[idx].data.fd;
      //查看文件描述符是不是listenfd
      if (fd == _acceptor.fd()) {
        if (_evtList[idx].events & EPOLLIN) {
          //处理新的连接
          handleNewConnection();
        }
      } else {
        if (_evtList[idx].events & EPOLLIN) {
          //处理老的连接
          handleMessage(fd);
        }
      }
    }
  }
}
void EventLoop::handleNewConnection() {
  int connfd = _acceptor.accept();
  if (connfd < 0) {
    perror("handleNewConnection accept");
    return;
  }
  addEpollReadFd(connfd);
  TcpConnectionPtr con(new TcpConnection(connfd));
  con->setNewConnectionCallback(_onNewConnectionCb);
  con->setMessageCallback(_onMessageCb);
  con->setCloseCallback(_onCloseCb);

  _conns[connfd] = con;
  con->handleNewConnectionCallback();
}

void EventLoop::handleMessage(int fd) {
  auto it = _conns.find(fd);
  if (it != _conns.end()) {
    bool flag = it->second->isClosed();
    if (flag) {
      it->second->handleCloseCallback();
      delEpollReadFd(fd);
      _conns.erase(it);
    } else {
      it->second->handleMessageCallback();
    }

  } else {
    cout << "连接不存在" << endl;
    return;
  }
}

int EventLoop::createEpollFd() {
  int fd = ::epoll_create(100);
  if (fd < 0) {
    perror("epoll_create");
    return fd;
  }
  return fd;
}

void EventLoop::addEpollReadFd(int fd) {
  struct epoll_event evt;
  evt.events = EPOLLIN;
  evt.data.fd = fd;

  int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
  if (ret < 0) {
    perror("EPOLL_CTL_ADD");
    return;
  }
}

void EventLoop::delEpollReadFd(int fd) {
  struct epoll_event evt;
  evt.events = EPOLLIN;
  evt.data.fd = fd;

  int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
  if (ret < 0) {
    perror("epoll_ctl del");
    return;
  }
}

void EventLoop::setNewConnectionCallback(TcpConnectionCallback &&cb) {
  _onNewConnectionCb = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb) {
  _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb) {
  _onCloseCb = std::move(cb);
}
