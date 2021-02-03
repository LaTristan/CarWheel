#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
using namespace std;
struct COORD{
    double x;
    double y;
    COORD(double a,double b):x(a),y(b){}
    COORD():x(0),y(0){}
};
double Dist(COORD p1,COORD p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
class Classify{
    private:
    vector<vector<COORD>>dataset;
    vector<COORD>centers;
    vector<double>weights;
    double tolerance;
    public:
    Classify(double t):tolerance(t){}
    void insert(COORD coord,double w){
        for(int i=0;i<centers.size();i++){

        }
    }
};
#endif