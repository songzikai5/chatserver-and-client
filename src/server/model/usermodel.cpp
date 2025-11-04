#include "usermodel.hpp"
#include"CommonConnectionPool.h"

//user表的增加方法
bool UserModel::insert(User &user){
    //获取连接池
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "insert into user(name, password, state) \
    values('%s', '%s', '%s')", user.getName().c_str(),
        user.getPassword().c_str(),user.getState().c_str()); 
    if(sp->update(sql)){
        //获取插入成功的用户数据生成的主键id
        user.setId(sp->getInsertId());
        return true;
    }
    return false;
}

//根据用户号码查询用户信息
User UserModel::query(int id){
    //获取连接池
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "SELECT * FROM user WHERE id = %d", id);
    MYSQL_RES *res=sp->query(sql);

    // 解析结果集（只取第一行，因为id是主键，唯一）
    MYSQL_ROW row = mysql_fetch_row(res);

    if(res==nullptr||row == nullptr){
        //查询失败
        return User();
    }

    User user;
    user.setId(atoi(row[0]));       // 第0列是id（int）
    user.setName(row[1]);           // 第1列是name（string）
    user.setPassword(row[2]);       // 第2列是password
    user.setState(row[3]);          // 第3列是state
    mysql_free_result(res);     // 释放结果集（避免内存泄漏）
    return user;

}

//更新用户的状态信息
bool UserModel::updateState(User user){
    //获取连接池
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "update user set state ='%s' \
        WHERE id = %d", user.getState().c_str(),user.getId());
    return sp->update(sql);
}

//重置用户的状态信息
void UserModel::resetState(){
    //获取连接池
    ConnectionPool *cp=ConnectionPool::getConnectionPool();
    shared_ptr<Connection>sp=cp->getConnection();
    //组合sql语句
    char sql[1024]={0};
    sprintf(sql, "update user set state ='offline' \
        WHERE state ='online'");
    sp->update(sql);
}

    