#include "offlinemessagemodel.hpp"
#include "CommonConnectionPool.h"

//存储用户的离线消息
void OfflineMsgModel::insert(int userid,string msg){
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "insert into offlinemessage \
    values(%d,'%s')", userid,msg.c_str()); 
    sp->update(sql);
}

//删除用户的离线消息
void OfflineMsgModel::remove(int userid){
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "delete from offlinemessage \
        where userid=%d", userid); 
    sp->update(sql);
}

//查询用户的离线消息
vector<string> OfflineMsgModel::query(int userid){
    //获取连接池
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "SELECT message FROM offlinemessage \
        WHERE userid = %d", userid);
    MYSQL_RES *res=sp->query(sql);
    if(res==nullptr){
        //查询失败
        return vector<string>();
    }

    vector<string> vec;

    // 解析结果集（只取第一行，因为id是主键，唯一）
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res))!=nullptr){
        vec.push_back(row[0]);
    }
    mysql_free_result(res);
    return vec;
     

}