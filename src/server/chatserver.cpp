#include "chatserver.hpp"
#include "chatservice.hpp"
#include"json.hpp"
#include<functional>
#include<string>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,//反应堆
        const InetAddress& listenAddr,//IP+Port
        const string& nameArg)//服务器名字
        :_server(loop,listenAddr,nameArg)
        ,_loop(loop)
    {
        //注册连接回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection
            ,this,_1));
        
        //注册消息回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage,
            this,_1,_2,_3));
        
        //设置线程数量
        _server.setThreadNum(4);
    }

//启动服务
void ChatServer::start(){
    _server.start();
}

//上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr&conn){
    //客户端断开连接
    if(!conn->connected()){
        ChatService::instance()->clientCloaeException(conn);
        conn->shutdown();
    }
}

//上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr&conn
    ,Buffer*buffer
    ,Timestamp time)
{
    string buf =buffer->retrieveAllAsString();
    //数据的反序列化
    json js=json::parse(buf);
    auto msgHandler=ChatService::instance()->
        getHandler(js["msgid"].get<int>());
    msgHandler(conn,js,time);
}

