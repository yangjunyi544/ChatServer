#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <iostream>
#include <string>
#include <functional>
using namespace std;
using namespace muduo;
using namespace muduo::net;

/*
服务器类，基于muduo库开发
1.组合TcpServer对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要什么参数，输出Chatserver的构造函数
4.在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
5.设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/
 class ChatServer
 {
 public:
    // 初始化TcpServer
    ChatServer(EventLoop *loop,
        const InetAddress &listenAddr,
        const string& nameArg)
        :_server(loop, listenAddr, nameArg),_loop(loop)
    {
        // 通过绑定器设置回调函数
        _server.setConnectionCallback(bind(&ChatServer::onConnection,
            this, _1));
        _server.setMessageCallback(bind(&ChatServer::onMessage,
            this, _1, _2, _3));
        // 设置EventLoop的线程个数
        _server.setThreadNum(4);
    }
    // 启动ChatServer服务
    void start()
    {
        _server.start();
    }
 private:
    // TcpServer绑定的回调函数，当有新连接或连接中断时调用
    void onConnection(const TcpConnectionPtr &conn)
    {
        if(conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << " -> " <<
              conn->localAddress().toIpPort() << " state:online" << endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << " -> " <<
              conn->localAddress().toIpPort() << " state:offline" << endl;
            conn->shutdown();
            //_loop->quit();
        }

    }
    // TcpServer绑定的回调函数，当有新数据时调用
    void onMessage(const TcpConnectionPtr &conn,
        Buffer *buffer,
        Timestamp time)
    {
       string buf = buffer->retrieveAllAsString();
       cout << "recv data:" << buf << " time:" << time.toString() << endl;
       conn->send(buf);
    }
 private:
    TcpServer _server;
    EventLoop *_loop; // epoll
 };


 int main()
 {
    EventLoop loop;
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop,addr,"ChatServer");

    server.start();
    loop.loop();

    return 0;
 }