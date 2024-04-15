// header_template.h
#ifndef INETADDRESS_H
#define INETADDRESS_H
#include <arpa/inet.h>
#include <string>

using std::string;
class InetAddress {
public:
  InetAddress(const string &ip, unsigned short port);
  InetAddress(const struct sockaddr_in &addr);
  ~InetAddress();
  string ip() const;
  unsigned short port() const;
  const struct sockaddr_in *getInetAddrPtr() const;

private:
  struct sockaddr_in _addr;
};

#endif /* INETADDRESS_H */
