#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_
#include <iostream>
#include <time.h>
using namespace std;
class TimeStamp{
    private:
    string id;
    time_t start;
    time_t end;
    public:
    TimeStamp(string id):id(id){
        start=clock();
        cout<<"-Established-"<<endl;
    }
    ~TimeStamp(){
        end=clock();
        cout<<"-Name-"<<id<<"-Duration:"<<end-start<<endl;
    }
};
#endif