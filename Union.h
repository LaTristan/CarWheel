#ifndef _UNION_H_
#define _UNION_H_

#include <vector>
#include <iostream>
#include <map>
using namespace std;

struct Coordinate{
    double x;
    double y;
    Coordinate(double a,double b):x(a),y(b){}
    Coordinate():x(0),y(0){}
};
class Union{
    private:
    //vector<Coordinate>part;
    vector<vector<Coordinate> >dataset;
    vector<Coordinate>centers;
    vector<double>weights;
    double tolerance;
    public:
    Union(double t){
        tolerance=t;
    }
    void toStart(Coordinate first,double w){
        vector<Coordinate>a;
        a.push_back(first);
        dataset.push_back(a);
        centers.push_back(first);
        weights.push_back(w);
    }
};
#endif