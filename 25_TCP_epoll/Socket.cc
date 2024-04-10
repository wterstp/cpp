#include "Socket.h"
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
Socket::Socket() {
  _fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0) {
    perror("socket");
    return;
  }
}
Socket::Socket(int fd) : _fd(fd) {}

Socket::~Socket() { close(_fd); }
int Socket::fd() const { return _fd; }
void Socket::shutDownWrite() {
  int ret = shutdown(_fd, SHUT_WR);
  if (ret) {
    perror("shutdown");
    return;
  }
}
