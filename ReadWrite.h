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
enum GEOM{POINT,LINE,POLYGON,POLYLINE};
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
    void Clear(){
        ofstream ofs(f);
        ofs<<"";
        ofs.close();
    }
    ReadWrite(MODE mode,string filePath):mode(mode),f(filePath){}
    void WriteLine(string line){
        ofstream ofs(f,ios::app);
        ofs<<line<<endl;
        ofs.close();
    }
    vector<string> ReadLines(){
        ifstream ifs(f,ios::in);
        vector<string>res;
        string tmp;
        while(getline(ifs,tmp)){
            res.push_back(tmp);
        }
        ifs.close();
        return res;
    }
    void WriteWktLine(string line,GEOM geom);
    protected:
    void Execute();
};
void ReadWrite::Execute(){
    cout<<"--Execute"<<endl;
}
void ReadWrite::WriteWktLine(string line,GEOM geom){
    cout<<"Export to file "<<f<<" with WKT Lines"<<endl;
    ofstream ofs(f,ios::app);
    for(int i=0;i<line.size();i++){
        if(line[i]==','){
            line.replace(i,1," ");
        }
    }
    istringstream iss(line);
    switch(geom){
        case POINT:
        cout<<"Point:"<<endl;
        double x,y;
        iss>>x>>y;
        ofs<<"POINT("<<x<<" "<<y<<")"<<endl;
        break;
        case POLYLINE:
        break;
        case LINE:
        break;
        case POLYGON:
        break;
        default:
        cout<<"Error"<<endl;
        return;
    }
    ofs.close();
    return;
}
#endif