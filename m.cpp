#include <iostream>
#include "TimeStamp.h"
#include "DecisionTree.h"
using namespace std;

void move(){
    TimeStamp t("first");
}
int main(){
    move();
    cout<<"Hello World!"<<endl;
    DecisionTree tree("5_data.csv",7);
    tree.Process();
    tree.Predict();
    return 0;
}