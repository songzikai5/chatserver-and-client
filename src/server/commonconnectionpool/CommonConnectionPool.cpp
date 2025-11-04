#include"CommonConnectionPool.h"



//线程安全的懒汉单例函数接口
ConnectionPool* ConnectionPool::getConnectionPool(){
    static ConnectionPool pool;
    return &pool;
}

//连接池构造函数
ConnectionPool::ConnectionPool(){
    //加载配置项
    if(!loadConfigFile()){
        return;
    }

    //创建初始数量的连接
    for(int i=0;i<_initSize;++i){
        Connection*p=new Connection();
        p->connect(_ip,_port,_username,_password,_dbname);
        p->refreshAliveTime();
        _connectionQue.push(p);
        
        _connectionCnt++;
    }

    //启动一个新线程,作为连接的生产者
    thread produce(std::bind(&ConnectionPool::produceConnectionTask,this));
    //std::thread produce([this]() { this->produceConnectionTask(); });
    produce.detach();


    //启动一个新的定时线程,扫描多余的连接,去除超过maxIdleTime时间的连接
    thread scanner(std::bind(&ConnectionPool::scannerConnectionTask,this));
    scanner.detach();
    
}; 

//从配置文件中加载配置项
bool ConnectionPool::loadConfigFile(){
    FILE *pf=fopen("../config/mysql.conf","r");
    if(pf==nullptr){
        LOG("mysql.conf file is not exist");
        return false;
    }

    while(!feof(pf)){
        char line[1024]={0};
        fgets(line,1024,pf);
        string str=line;
        int idx=str.find('=',0);
        //无效配置
        if(idx==-1){
            continue;;
        }

        // password=Root123!\n
        // 查找换行符'\n'的位置（从'='之后开始找，因为值可能包含其他字符）
        int endidx = str.find('\n', idx);
        // 提取key：从0到'='的位置（不包含'='）
        string key = str.substr(0, idx);
        // 提取value：从'='后一位开始，到换行符前一位结束（去除换行符）
        string value = str.substr(idx + 1, endidx - idx - 1);
        //cout<<key<<"="<<value<<endl;

        if(key=="ip"){
            _ip=value;
        }else if(key=="port"){
            _port=atoi(value.c_str());
        }else if(key=="username"){
            _username=value;
        }else if(key=="password"){
            _password=value;
        }else if(key=="dbname"){
            _dbname=value;
        }else if(key=="initSize"){
            _initSize=atoi(value.c_str());
        }else if(key=="maxSize"){
            _maxSize=atoi(value.c_str());
        }else if(key=="maxIdleTime"){
            _maxIdleTime=atoi(value.c_str());
        }else if(key=="connectionTimeOut"){
            _connectionTimeout=atoi(value.c_str());
        }
    }
    return true;
}

//运行在独立的线程中,专门负责生产新连接
void ConnectionPool::produceConnectionTask(){
    
    while(1){
        unique_lock<mutex> lock(_queueMutex);
        while(!_connectionQue.empty()){
            cv.wait(lock);
        }

        if(_connectionCnt<_maxSize){
            Connection*p=new Connection();
            p->connect(_ip,_port,_username,_password,_dbname);
            p->refreshAliveTime();
            _connectionQue.push(p);
            _connectionCnt++;
        }
        //通知消费者线程可以来消费
        cv.notify_all();
    }
}

//给外部提供接口,从连接池中获取一个可用的空闲连接
shared_ptr<Connection> ConnectionPool::getConnection(){
    unique_lock<mutex> lock(_queueMutex);
    while(_connectionQue.empty()){
        if(cv_status::timeout== cv.wait_for(lock,chrono::milliseconds(_connectionTimeout))){
            if(_connectionQue.empty()){
                LOG("获取连接超时.....");
                return nullptr;
            }
        }
        
    }

    /*
    shared_ptr 智能指针析构时，会把 connection 资源直接 delete 掉，
    相当于调用 connection 的析构函数，connection 就被 close 掉了。
    这里需要自定义 shared_ptr 的释放资源的方式，
    把 connection 直接归还到 queue 当中
    */

    shared_ptr<Connection> sp(_connectionQue.front(),
        [&](Connection *pcon){
            //要考虑队列的线程安全
            lock_guard<mutex>lock(_queueMutex);
            pcon->refreshAliveTime();
            _connectionQue.push(pcon);
        });
    _connectionQue.pop();

    //消费后,通知生产者线程检查一下队列是否为空
    cv.notify_all();
    
    return sp;
}


//扫描多余的连接,去除超过maxIdleTime时间的连接
void ConnectionPool::scannerConnectionTask(){
    for(;;){
        //模拟定时效果
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));

        //扫描队列,释放多于连接
        unique_lock<mutex> lock(_queueMutex);
        while(_connectionCnt>_initSize&&
            _connectionQue.front()->getAlivetime()>=(_maxIdleTime*1000)){
                Connection*p= _connectionQue.front();
                _connectionQue.pop();
                _connectionCnt--;
                delete p;
            }

    }
}