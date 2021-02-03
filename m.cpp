#include <iostream>
#include "TimeStamp.h"
#include "DecisionTree.h"
#include <vector>
#include <math.h>
using namespace std;

#define k 4  //聚类数，可在kmeans算法处作参数

struct Tuple  //数据结构
{
	float attr1;//可看作x
	float attr2;//可看作y
};

float Dist(Tuple t1,Tuple t2)//欧式距离
{
	return sqrt((t1.attr1 - t2.attr1)*(t1.attr1 - t2.attr1) + (t1.attr2 - t2.attr2)*(t1.attr2 - t2.attr2));
}
 
//决定该样本属于哪一个聚类，传入的是聚类的质心(也是一个组，看作x，y)和一个样本，返回的是label;
int clusterofTuple(Tuple means[],Tuple tuple)
{
	float distance = Dist(means[0],tuple);
	int label = 0;
	for (int i = 0; i < k; i++)
	{
		if (Dist(means[i],tuple)<distance)
		{
			label = i;
		}
	}
	return label; //找最近质心
 
}
 
//获得蔟集的平方误差,用来判断是否还需要继续迭代，传入的是蔟集的质心，以及所有归类好的样本,装着每个蔟集的容器数组，计算该聚类到自己质心的距离，所有距离的加和，返回所有的平方误差
float getVar(Tuple means[],vector<Tuple> cluster[])
{
	float var = 0;
	for (int i = 0; i < k; i++)
	{
		vector<Tuple> t = cluster[i];
		for (int j = 0; j < t.size(); j++)
		{
			var += Dist(means[i], t[j]);
		}
	}
	return var;
}

float ads(float oldvar,float newvar){  //计算平方差变化
	float p;
	p=newvar-oldvar;
	return p;

}
//计算当前蔟集的质心，输入的是一个蔟集的容器，质心的计算就是对于两个属性累加后除以个数求平均，然后返回质心，所以也要初始化一个质心Tuple t
Tuple getMeans(vector<Tuple> cluster)
{
	Tuple t;
	int num = cluster.size();
	float meanX = 0, meanY = 0;
	for (int i = 0; i < num; i++)
	{
		meanX += cluster[i].attr1;
		meanY += cluster[i].attr2;
	}
	t.attr1 = meanX / num;
	t.attr2 = meanY / num;
	return t;
 
}
 
void Kmeans(vector<Tuple> tuples)  //kmeans算法
{    //定义与初始化
    //首先是要定义一个放置分好的蔟，那就是容器组咯，一个容器放一个蔟
    //然后还要有放k个质心的数组
	vector<Tuple> cluster[k];//容器组
	Tuple means[k];//放k个质心的数组
	//首先设置默认的质心，就是每个组分别是所有tuples里面最前面三个;
	for(int i = 0; i < k; i++)
	{
		means[i].attr1 = tuples[i].attr1;
		means[i].attr2 = tuples[i].attr2;
	}
 
	//第一次计算距离，进行分类，得到第一次的类标，容器的话是直接用push_back放置进去
	int label = 0;
	for (int i = 0; i < tuples.size(); i++)
	{
		label = clusterofTuple(means, tuples[i]);
		cluster[label].push_back(tuples[i]);
 
	}
 
    //输出刚开始的蔟
	for (int i = 0; i < k; i++)
	{
		cout << "the num of " << i << endl;
		vector<Tuple> t = cluster[i];
		for (int j = 0; j<t.size(); j++)
		{
			cout << t[j].attr1 << " " << t[j].attr2 << " " << endl;
		}
	}
 
	float oldvar = 0;//上一轮平方差
    float newvar = getVar(means,cluster);
	//循环迭代
		while (ads(oldvar,newvar)>10)  //结束条件,可修改
		{   
			//1先计算新的k个质心
			for (int i = 0; i < k; i++)
			{
				means[i] = getMeans(cluster[i]);
			}
			//2清空分号蔟的容器，待会才可以根据新的质心重新分配
			for (int i = 0; i < k; i++)
			{
				cluster[i].clear();
			}
			//3根据新的质心，对于原来传入的所有数据重新分配
			for (int i = 0; i < tuples.size(); i++)
			{
				label = clusterofTuple(means, tuples[i]);
				cluster[label].push_back(tuples[i]);
			}
			//4最后输出
			for (int i = 0; i < k; i++)
			{
				vector<Tuple> t = cluster[i];
				for (int j = 0; j < t.size(); j++)
				{
					cout << t[j].attr1 << t[j].attr2 << endl;
				}
			}
		}
 
}

vector<Tuple> input(vector<Tuple> &tuples){  //数据输入，可改读入
	Tuple tuple[3];
	tuple[0].attr1=1;tuple[0].attr2=1;
	tuple[1].attr1=1;tuple[1].attr2=2;
	tuple[2].attr1=7;tuple[2].attr2=10;
	tuples.push_back(tuple[0]);
	tuples.push_back(tuple[1]);
	tuples.push_back(tuple[2]);
	return tuples;
}
 
int main()
{
    vector<Tuple> tuples;
	input(tuples);
	
	Kmeans(tuples);
    system("pause");
	return 0;
}