#include "EchoServer.h"
#include "TcpConnection.h"
#include <iostream>

#include"../../head/SplitTool.h"
#include"../../head/Speech_recognition.h"
using std::cout;
using std::endl;

MyTask::MyTask(const string &msg, const TcpConnectionPtr &con)
: _msg(msg)
, _con(con)
{

}
void MyTask::process()
{
    _con->sendInLoop(_msg);
}

EchoServer::EchoServer(size_t threadNum, size_t queSize
                       , const string &ip
                       , unsigned short port)
: _pool(threadNum, queSize)
, _server(ip, port)
,_cache(1000)
{

}

EchoServer::~EchoServer()
{

}

void EchoServer::start()
{
    _pool.start();
    using namespace  std::placeholders;
    _server.setAllCallback(std::bind(&EchoServer::onNewConnection, this, _1)
                           , std::bind(&EchoServer::onMessage, this, _1)
                           , std::bind(&EchoServer::onClose, this, _1));
    _server.start();
}

void EchoServer::stop()
{
    _pool.stop();
    _server.stop();
}

void EchoServer::onNewConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has connected!" << endl;
}

void EchoServer::onMessage(const TcpConnectionPtr &con)
{
    string msg = con->receive();

    cout << ">>recv client msg = " << msg << endl;
    msg[msg.size()] = '\0';
    // 做业务逻辑的处理
    // 在此处，需要将接受到的数据msg打包给线程池进行处理
    std::string result;
    if (msg[0] = '0')
    {

        msg.erase(msg.begin());
        std::vector<std::string> vec = _cache.get(msg);
        for (auto &i : vec)
        {
            result = result + i + "\n";
        }
    }
    cout<<"result:"<<result<<endl;
    MyTask task(result, con);
    _pool.addTask(std::bind(&MyTask::process, task));
}

void EchoServer::onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!" << endl;
}
