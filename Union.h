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
    protected:
    void logIn(Coordinate item,double w){
        Coordinate after;
        for(int i=0;i<centers.size();i++){
            after.x=(item.x*w+centers[i].x*weights[i])/(item.x+centers[i].x);
            after.y=(item.y*w+centers[i].y*weights[i])/(item.y+centers[i].y);
            if((centers[i].x-after.x)*(centers[i].x-after.x)+(centers[i].y-after.y)*(centers[i].y-after.y)<tolerance){
                centers[i].x=after.x;
                centers[i].y=after.y;
                weights[i]+=w;
                dataset[i].push_back(item);
                break;
            }
            else{
                vector<Coordinate>ano;
                ano.push_back(item);
                dataset.push_back(ano);
                weights.push_back(w);
                centers.push_back(item);
                break;
            }
        }
    }
};
#endif