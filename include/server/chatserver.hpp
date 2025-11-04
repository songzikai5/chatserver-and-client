#ifndef CHATSERVER_H
#define CHATSERVER_H

#include"json.hpp"

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;


class ChatServer{
public:
    ChatServer(EventLoop* loop,//反应堆
        const InetAddress& listenAddr,//IP+Port
        const string& nameArg);


    void start();

private:
    
    void onConnection(const TcpConnectionPtr&conn);
    void onMessage(const TcpConnectionPtr&conn
        ,Buffer*buffer
        ,Timestamp time);
    
    TcpServer _server;
    EventLoop *_loop;
};



#endif