#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"
#include <string.h>
#include <functional>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg)
            :_server(loop, listenAddr, nameArg)
            ,_loop(loop)
{
    // 通过绑定器设置回调函数
    _server.setConnectionCallback(bind(&ChatServer::onConnection,this, _1));
    
    _server.setMessageCallback(bind(&ChatServer::onMessage,this, _1, _2, _3));
    
    // 设置EventLoop的线程个数
    _server.setThreadNum(4);
}

// 启动ChatServer服务
void ChatServer::start()
{
    _server.start();
}

void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if(!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

//上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
        Buffer *buffer,
        Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    //数据反序列化
    json js = json::parse(buf);
    //达到目的：完全解耦网络模块代码和业务模块的代码
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    //回调消息绑定事件处理器，来执行相应的业务
    msgHandler(conn,js,time);
}
