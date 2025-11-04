#ifndef CONNECTION_H
#define CONNECTION_H
/*
实现Mysql数据库的增删改查工作
*/
#include <mysql.h>
#include <string>
#include <iostream>
#include  <ctime>
#include "log.h"


using namespace std;

class Connection{
public:
    
    // 初始化数据库连接
    Connection();
    
    // 释放数据库连接资源
    ~Connection();
    
    // 连接数据库
    bool connect(string ip, 
        unsigned short port, 
        string user, 
        string password,
        string dbname);

    // 更新操作 insert、delete、update
    bool update(string sql);
    
    // 查询操作 select
    MYSQL_RES* query(string sql);

    //刷新存货时间
    void refreshAliveTime(){
        _alivetime=clock();
    }

    clock_t getAlivetime(){
        return clock()-_alivetime;
    }
    
    int getInsertId() {
        return mysql_insert_id(_conn); // 显式使用当前连接的句柄
    }

private:
    
    MYSQL *_conn; // 表示和MySQL Server的一条连接
    clock_t _alivetime;//记录进入空闲状态的时间(队列时间)
};

#endif