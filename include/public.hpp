#ifndef PUBLIC_H
#define PUBLIC_H

/*
server和client的公共文件
*/
enum EnMsgType{
    
    //登录消息{"msgid":LOGIN_MSG,"id":22,"password":"1234"} 
    LOGIN_MSG=1,

    LOGIN_MSG_ACK,//登录响应消息

    LOGINOUT_MSG, // 注销消息

    //注册消息{"msgid":REG_MSG,"name":zs,"password":"1234"} 
    REG_MSG,   

    REG_MSG_ACK,//注册响应消息 
    
    //一对一聊天消息{"msgid":ONE_CHAT_MSG,"id":22,
    //    "from":"zhang san","to":3,msg:"xxxxxx"} 
    ONE_CHAT_MSG,

    //添加好友{"msgid":ADD_FRIEND_MSG,"id":zs,"friendid":"1234"}
    ADD_FRIEND_MSG,

    CREATE_GROUP_MSG,//创建群组
    ADD_GROUP_MSG,//加入群组
    GROUP_CHAT_MSG,//群聊天

    
};


#endif