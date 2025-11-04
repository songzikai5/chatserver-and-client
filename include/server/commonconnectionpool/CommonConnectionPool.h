#ifndef COMMONCONNECTION_H
#define COMMONCONNECTION_H
#include <string>
#include <queue>
#include <mutex>
#include <iostream>
#include <atomic>
#include <thread>
#include <memory>
#include <functional>
#include <condition_variable>
#include "Connection.h"

#include "log.h"

using namespace std;
/*
实现连接池功能模块
*/

class ConnectionPool{


public:

    //获取连接池对象示例
    static ConnectionPool* getConnectionPool();

    //给外部提供接口,从连接池中获取一个可用的空闲连接
    shared_ptr<Connection> getConnection();

    //给外部提供一个线程

    string getIp(){
        return this->_ip;
    }

private:
    //单例模式,构造函数私有化
    ConnectionPool(); 

    //从配置文件中加载配置项
    bool loadConfigFile();

    //运行在独立的线程中,专门负责生产新连接
    void produceConnectionTask();

    //扫描多余的连接,去除超过maxIdleTime时间的连接
    void scannerConnectionTask();


    string _ip;             //mysql的ip地址
    unsigned short _port;   //mysql的端口号
    string _username;       //mysql登录用户名
    string _password;       //mysql登录密码
    string _dbname;         //数据库名字
    int _initSize;          //连接池的初始连接量
    int _maxSize;           //连接池的最大连接量
    int _maxIdleTime;       //连接池最大空闲时间
    int _connectionTimeout; //连接池获取连接的超时时间

    queue<Connection*>_connectionQue; //存储mysql连接的队列
    mutex _queueMutex;      //维护连接队列的线程安全互斥锁
    atomic_int _connectionCnt;//记录所创建的connection的总连接数量
    /*
    std::atomic_int 是 C++11 标准中 <atomic> 头文件提供的原子类型，
    专门用于对 int 类型数据进行原子操作。它是 std::atomic<int> 的 
    typedef（类型别名）
    */

    condition_variable cv;//设置条件变量,用于连接生产线程和连接消费线程的通信



};

#endif