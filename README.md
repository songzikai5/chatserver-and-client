# chatserver-and-client
基于muduo库编写的聊天器和对应的客户端

编译依赖mysql库,muduo库,hiredis库,nginx库

运行前需要启动mysql,nginx,rides的运行

## 编译

```bash
cd build
cmake ..
make

#运行服务器
../bin/ChatServer ip地址 端口号

#运行客户端
../bin/ChatClient ip地址 端口号
```