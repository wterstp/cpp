#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>

using std::cout;
using std::endl;
ThreadPool *gPool = nullptr;
class MyTask {
public:
  MyTask(const std::string &msg, const TcpConnectionPtr &con)
      : _msg(msg), _con(con) {}

  void process() { _con->sendInLoop(_msg); }

private:
  string _msg;
  TcpConnectionPtr _con;
};

void onNewConnection(const TcpConnectionPtr &con) {
  cout << con->toString() << " has connected!" << endl;
}

//文件描述符可读(消息到达)
void onMessage(const TcpConnectionPtr &con) {
  string msg = con->receive();
  cout << ">>recv client msg = " << msg << endl;
  //做业务逻辑的处理
  //...
  //...
  //处理完成之后，讲结果返回给客户端
  con->send(msg);
}

//连接断开
void onClose(const TcpConnectionPtr &con) {
  cout << con->toString() << " has closed!" << endl;
}

void test() {
  ThreadPool pool(4, 10);
  pool.start();
  gPool = &pool;
  TcpServer server("127.0.0.1", 8888);
  server.setAllCallback(std::move(onNewConnection), std::move(onMessage),
                        std::move(onClose));
  server.start();
}

int main(int argc, char *argv[]) {
  test();
  return 0;
}
