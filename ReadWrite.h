#ifndef _READWRITE_H_
#define _READWRITE_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "Classify.h"
using namespace std;
enum MODE{read,write,check};
struct DataBlock{
    int id;
    string classname;
    COORD coord;
    DataBlock(){
        coord.x=0.0;
        coord.y=0.0;
        id=0;
        classname="default";
    }
};
class ReadWrite{
    private:
    ifstream inFile;
    ofstream outFile;
    int loop;
    string f;
    protected:
    MODE mode;
    public:
    ReadWrite(MODE mode,string filePath):mode(mode),f(filePath){}
    void Execute();
};

void ReadWrite::Execute(){
    cout<<"--Execute"<<endl;
}
#endif