#ifndef LOG_H
#define LOG_H
using namespace std;

#define LOG(str) \
    cout<<__FILE__<<":"<<__LINE__<<" "<< \
    __TIMESTAMP__<<":"<<str<<endl;

#endif