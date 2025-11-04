// #include "chatserver.hpp"
// #include <iostream>
// using namespace std;

// int main(){
//     EventLoop loop;
//     InetAddress addr("127.0.0.1",6000);

//     ChatServer server(&loop,addr,"ChatServer");
//     server.start();
//     loop.loop();


//     return 0;
// }

#include "chatserver.hpp"
#include <iostream>
#include<signal.h>
#include "chatservice.hpp"
using namespace std;

void resetHandler(int){
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatClient 127.0.0.1 6000" << endl;
        exit(-1);
    }

    //解析ip和port
    char *ip=argv[1];
    uint16_t port=atoi(argv[2]);

    signal(SIGINT,resetHandler);

    EventLoop loop;
    InetAddress addr(ip,port);

    ChatServer server(&loop,addr,"ChatServer");
    server.start();
    loop.loop();


    return 0;
}



/*测试2*/
// #include <iostream>
// #include <mysql.h>
// //#include "Connection.h"
// #include "CommonConnectionPool.h"
// using namespace std;

// int main(){
    
    
//     clock_t begin =clock();


//     thread t1([](){
//         ConnectionPool *cp=ConnectionPool::getConnectionPool();
//         for(int i=0;i<1250;++i){
//             shared_ptr<Connection>sp=cp->getConnection();
//             char sql[1024]={0};
//             sprintf(sql, "insert into users(username, age, register_time) \
//             values('%s', %d, '%s')", "zhangsan", 20, "2008-01-01"); 
//             sp->update(sql);  
//         }
//     });
//     thread t2([](){
//         ConnectionPool *cp=ConnectionPool::getConnectionPool();
//         for(int i=0;i<1250;++i){
//             shared_ptr<Connection>sp=cp->getConnection();
//             char sql[1024]={0};
//             sprintf(sql, "insert into users(username, age, register_time) \
//             values('%s', %d, '%s')", "zhangsan", 20, "2008-01-01"); 
//             sp->update(sql);  
//         }
//     });
//     thread t3([](){
//         ConnectionPool *cp=ConnectionPool::getConnectionPool();
//         for(int i=0;i<1250;++i){
//             shared_ptr<Connection>sp=cp->getConnection();
//             char sql[1024]={0};
//             sprintf(sql, "insert into users(username, age, register_time) \
//             values('%s', %d, '%s')", "zhangsan", 20, "2008-01-01"); 
//             sp->update(sql);  
//         }
//     });
//     thread t4([](){
//         ConnectionPool *cp=ConnectionPool::getConnectionPool();
//         for(int i=0;i<1250;++i){
//             shared_ptr<Connection>sp=cp->getConnection();
//             char sql[1024]={0};
//             sprintf(sql, "insert into users(username, age, register_time) \
//             values('%s', %d, '%s')", "zhangsan", 20, "2008-01-01"); 
//             sp->update(sql);  
//         }
//     });

//     t1.join();
//     t2.join();
//     t3.join();
//     t4.join();


//     // for(int i=0;i<10000;++i){
    
//         // Connection conn;
//         // char sql[1024]={0};
//         // sprintf(sql, "insert into users(username, age, register_time) \
//         //     values('%s', %d, '%s')", "zhangsan", 20, "2008-01-01"); 
//         // conn.connect("127.0.0.1",3306,"root","Root123!","chat");
//         // conn.update(sql);
        
//     //     // shared_ptr<Connection>sp=cp->getConnection();
//     //     // char sql[1024]={0};
//     //     // sprintf(sql, "insert into users(username, age, register_time) \
//     //     // values('%s', %d, '%s')", "zhangsan", 20, "2008-01-01"); 
//     //     // sp->update(sql);  
//     // }


//     clock_t end =clock();
    
//     cout<<end-begin<<"ms"<<endl;
//     return 0;
// }