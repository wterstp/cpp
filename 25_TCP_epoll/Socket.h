// header_template.h
#ifndef SOCKET_H
#define SOCKET_H

#include "NonCopyable.h"

class Socket : NonCopyable {
public:
  Socket();
  explicit Socket(int fd);
  ~Socket();
  int fd() const;
  void shutDownWrite();

private:
  int _fd;
};

#endif /* SOCKET_H */
