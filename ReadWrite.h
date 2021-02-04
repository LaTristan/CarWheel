#ifndef _READWRITE_H_
#define _READWRITE_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
enum MODE{read,write,check};
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