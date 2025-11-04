#include "friendmodel.hpp"
#include "CommonConnectionPool.h"

//添加好友关系
void FriendModel::insert(int userid,int friendid){
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "insert into friend \
    values(%d,%d)", userid,friendid); 
    sp->update(sql);
}

//返回用户好友列表 friendid name state
vector<User> FriendModel::query(int userid){
    //获取连接池
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "SELECT a.id,a.name,a.state FROM user a inner join \
        friend b on b.friendid=a.id where b.userid=%d", userid);
    MYSQL_RES *res=sp->query(sql);
    if(res==nullptr){
        //查询失败
        return vector<User>();
    }

    vector<User> vec;

    // 解析结果集（只取第一行，因为id是主键，唯一）
    MYSQL_ROW row;
    
    while((row = mysql_fetch_row(res))!=nullptr){
        User user;
        user.setId(atoi(row[0]));
        user.setName(row[1]);
        user.setState(row[2]);
        vec.push_back(user);
    }
    mysql_free_result(res);
    return vec;
}