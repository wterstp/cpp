// header_template.h
#ifndef SOCKETIO_H
#define SOCKETIO_H

class SocketIO {
public:
  explicit SocketIO(int fd);
  ~SocketIO();
  int readn(char *buf, int len);
  int writen(const char *buf, int len);
  int readLine(char *buf, int len);

private:
  int _fd;
};

#endif /* SOCKETIO_H */
