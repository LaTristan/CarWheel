#ifndef _DECISIONTREE_H_
#define _DECISIONTREE_H_

#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

struct TreeNode {
    string strAttribute;//属性名称
    string strArrivedValue;//属性特征值，试为访问的数值
    vector<TreeNode*>vecTreeChild;//子节点

    TreeNode() :strAttribute(""), strArrivedValue("") {}
    TreeNode(string f) :strAttribute(f) {}
    TreeNode(TreeNode& another) {
        this->strAttribute = another.strAttribute;
        this->strArrivedValue = another.strArrivedValue;
        this->vecTreeChild = another.vecTreeChild;
    }
};

class DecisionTree {
private:
    vector<vector<string>>vSamples;//初始数据
    vector<string>vStrItems;//单个样本临时存储
    vector<string>vAttributeRow;//属性行数据
    map<string, vector<string>>mapStrVecValues;//存储结构为map类型方便访问
    int tree_size = 0;
    TreeNode* root;//根节点
    int ColNum;//数据文件包含的维数
    string inFile;
protected:
    //清理树节点空间
    void ClearSpace(TreeNode* p) {
        if (p == NULL)
            return;
        for (vector<TreeNode*>::iterator it = p->vecTreeChild.begin(); it != p->vecTreeChild.end(); it++) {
            ClearSpace(*it);
        }
        delete p;
        tree_size++;
    }
    //递归打印
    void PrintTree(TreeNode* p, int depth) {
        ofstream ofs("Prediction.txt", ios::app);
        for (int i = 0; i < depth; i++) {
            cout << "    ";//按照树的深度先输出tab
            ofs << "---|";
        }
        if (!p->strArrivedValue.empty()) {
            ofs << p->strArrivedValue << endl;
            cout << p->strArrivedValue << endl;
            for (int i = 0; i < depth + 1; i++){
                cout << "---|";//按照树的深度先输出tab  
                ofs << "---|";
            }
        }
        ofs << p->strAttribute << endl;
        cout << p->strAttribute << endl;
        for (vector<TreeNode*>::iterator it = p->vecTreeChild.begin(); it != p->vecTreeChild.end(); it++) {
            PrintTree(*it, depth + 1);
        }
        ofs.close();
    }
public:
    //目前要修改数据维数需要修改标记的地方
    DecisionTree(string filePath,int MAX) {
        this->ColNum = MAX;
        root = NULL;
        inFile=filePath;

        string temp1, temp2, temp3, temp4, temp5, temp6;//这里规定每个样本需要限定维数，目前无法随意修改
        for (int i = 0; i < MAX; i++) {
            vStrItems.push_back("");
        }
        ifstream inFile(filePath, ios::in);
        string strRecv;
        int line = 0;//行数，用于区别样本ID
        while (getline(inFile, strRecv)) {
            for (int i = 0; i < strRecv.size(); i++) {
                if (strRecv.at(i) == ',')
                    strRecv.replace(i, 1, " ");
            }
            istringstream issIn(strRecv);
            /***************************************************************************************/
            issIn >> temp1 >> temp2 >> temp3 >> temp4 >> temp5 >> temp6;
            vector<string>temp = { to_string(line),temp1, temp2, temp3, temp4, temp5 ,temp6 };
            vSamples.push_back(temp);
            temp.clear();
            line++;
        }
        for (int j = 0; j < MAX; j++) {
            vAttributeRow.push_back(vSamples[0][j]);//载入类名
        }
    }
    ~DecisionTree() {
        ClearSpace(root);
        cout << "空间已清理" << endl;
        cout << tree_size << endl;
    }
    void BuildingMap() {//从vector二维数组载入map
        unsigned int i, j, k;
        bool exited = false;//后续判断是否需要退出循环
        vector<string> vecValues;
        for (i = 1; i < ColNum - 1; i++) {//按照列遍历  
            for (j = 1; j < vSamples.size(); j++) {
                for (k = 0; k < vecValues.size(); k++) {//从每个列寻找，没有不同属性时退出，即不计入计算
                    if (vecValues[k] == vSamples[j][i])
                        exited = true;
                }
                if (!exited) {
                    vecValues.push_back(vSamples[j][i]);
                }
                exited = false;
            }
            mapStrVecValues[vSamples[0][i]] = vecValues;
            vecValues.clear();
        }
    }
    //根据具体属值计算熵
    double Entropy(vector <vector <string> > remain_state, string attribute, string value, bool ifparent) {
        vector<int> count(2, 0);//yes和no两种数据，初始数目定0
        unsigned int i, j;
        bool done_flag = false;//哨兵值，检测是否退出
        for (j = 1; j < ColNum; j++) {
            if (done_flag)
                break;
            if (vAttributeRow[j] == attribute) {
                for (i = 1; i < remain_state.size(); i++) {
                    if ((!ifparent && remain_state[i][j] == value) || ifparent) {//ifparent记录是否算父节点，即需要创建子节点
                        if (remain_state[i][ColNum - 1] == "yes") {
                            count[0]++;
                        }
                        else
                            count[1]++;
                    }
                }
                done_flag = true;
            }
        }
        if (count[0] == 0 || count[1] == 0)//该实例下仅有一种结果时可以确认退出
            return 0;
        //具体计算熵 根据[+count[0],-count[1]],log2为底通过换底公式换成自然数底数  
        double sum = (double)count[0] + (double)count[1];
        double entropy = -count[0] / sum * log(count[0] / sum) / log(2.0) - count[1] / sum * log(count[1] / sum) / log(2.0);
        return entropy;
    }
    //计算按照属性attribute划分当前剩余实例的信息增益  
    double Gain(vector <vector <string> > remain_state, string attribute) {
        unsigned int j, k, m;
        //首先求不做划分时的熵  
        double dParentEntropy = Entropy(remain_state, attribute, "", true);
        double dChildrenEntropy = 0;
        //然后求做划分后各个值的熵  
        vector<string> vValues = mapStrVecValues[attribute];
        vector<double> vRatio;
        vector<int> vCountedValue;
        int tempI;
        for (m = 0; m < vValues.size(); m++) {
            tempI = 0;
            for (k = 1; k < ColNum - 1; k++) {
                if (vAttributeRow[k] == attribute) {
                    for (j = 1; j < remain_state.size(); j++) {
                        if (remain_state[j][k] == vValues[m]) {
                            tempI++;
                        }
                    }
                }
            }
            vCountedValue.push_back(tempI);
        }

        for (j = 0; j < vValues.size(); j++) {
            vRatio.push_back((double)vCountedValue[j] / (double)(remain_state.size() - 1));
        }
        double dTempEntropy;
        for (j = 0; j < vValues.size(); j++) {
            dTempEntropy = Entropy(remain_state, attribute, vValues[j], false);
            dChildrenEntropy += vRatio[j] * dTempEntropy;
        }
        return (dParentEntropy - dChildrenEntropy);
    }
    int GetAttributeNum(string strAttribute) {//根据名称寻找相应的维数
        for (int i = 0; i < ColNum; i++) {
            if (vSamples[0][i] == strAttribute)
                return i;
        }
        cerr << "can't find the numth of attribute" << endl;
        return 0;
    }
    //找出样例中占多数的正/负性  
    string MostCommonLabel(vector <vector <string>> remain_state) {
        int p = 0, n = 0;
        for (unsigned i = 0; i < remain_state.size(); i++) {
            if (remain_state[i][ColNum - 1]=="yes")
                p++;
            else
                n++;
        }
        if (p >= n)
            return "yes";
        else
            return "no";
    }
    //判断样例是否正负性都为label
    bool Label_All_Same(vector <vector <string> > remain_state, string label) {
        int count = 0;
        for (unsigned int i = 0; i < remain_state.size(); i++) {
            if (remain_state[i][ColNum - 1]==label)
                count++;
        }
        if (count == remain_state.size() - 1)
            return true;
        else
            return false;
    }

    //核心算法
//计算信息增益，DFS构建决策树  
//current_node为当前的节点  
//vvRemainSamples为剩余待分类的样例  
//vvRemainAttribute为剩余还没有考虑的属性  
//返回根结点指针  
    TreeNode* DFS_building(TreeNode* p, vector <vector <string> > vvRemainSamples, vector <string> vRemainAttribute) {
        //if(remain_state.size() > 0){  
            //printv(remain_state);  
        //}  
        if (p == NULL)
            p = new TreeNode();
        //先看搜索到树叶的情况  
        if (Label_All_Same(vvRemainSamples, "yes")) {//如果对应结果全部都是yes
            p->strAttribute = "yes";
            return p;
        }
        if (Label_All_Same(vvRemainSamples, "no")) {//如果对应结果全部都是no
            p->strAttribute = "no";
            return p;
        }
        if (vRemainAttribute.size() == 0) {//所有的属性均已经考虑完了,还没有分尽  
            string label = MostCommonLabel(vvRemainSamples);
            p->strAttribute = label;
            return p;
        }

        double MaxGain = 0, tempGain;
        vector <string>::iterator maxIterator = vRemainAttribute.begin();
        vector <string>::iterator it1;
        for (it1 = vRemainAttribute.begin(); it1 < vRemainAttribute.end(); it1++) {
            tempGain = Gain(vvRemainSamples, (*it1));
            if (tempGain > MaxGain) {
                MaxGain = tempGain;
                maxIterator = it1;
            }
        }
        //下面根据maxIterator指向的属性来划分当前样例，更新样例集和属性集  
        vector <string> vNewAttribute;
        vector <vector <string> > vvNewSamples;//暂存更新的二维数据
        for (vector <string>::iterator it2 = vRemainAttribute.begin(); it2 < vRemainAttribute.end(); it2++) {
            if ((*it2).compare(*maxIterator))
                vNewAttribute.push_back(*it2);
        }
        //确定了最佳划分属性，注意保存  
        p->strAttribute = *maxIterator;//容易迭代赋值
        vector <string> strValues = mapStrVecValues[*maxIterator];
        int iAttributeNum = GetAttributeNum(*maxIterator);
        vvNewSamples.push_back(vAttributeRow);
        for (vector <string>::iterator it3 = strValues.begin(); it3 < strValues.end(); it3++) {
            for (unsigned int i = 1; i < vvRemainSamples.size(); i++) {
                if (vvRemainSamples[i][iAttributeNum] == *it3) {
                    vvNewSamples.push_back(vvRemainSamples[i]);
                }
            }
            TreeNode* treeNewNode = new TreeNode();
            treeNewNode->strArrivedValue = *it3;
            if (vvNewSamples.size() == 0) {//表示当前没有这个分支的样例，当前的treeNewNode为叶子节点  
                treeNewNode->strAttribute = MostCommonLabel(vvRemainSamples);
            }
            else
                DFS_building(treeNewNode, vvNewSamples, vNewAttribute);
            //递归函数返回时即回溯时需要1 将新结点加入父节点孩子容器 2清除newSamples容器  
            p->vecTreeChild.push_back(treeNewNode);
            vvNewSamples.erase(vvNewSamples.begin() + 1, vvNewSamples.end());//注意先清空new_state中的前一个取值的样例，准备遍历下一个取值样例  
        }
        return p;
    }
    void Print() {
        PrintTree(root, 0);
    }
    //综合执行过程
    void Process() {
        vector<string>remain_attribute;
        ifstream ifs(inFile, ios::in);
        string AttName1, AttName2, AttName3, AttName4, AttName5;
        string s;
        getline(ifs, s);
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == ',')
                s.replace(i, 1, " ");
        }
        istringstream iss(s);
        iss >> AttName1 >> AttName2 >> AttName3 >> AttName4 >> AttName5;
        /***********************************************************************/
        remain_attribute.push_back(AttName1);
        remain_attribute.push_back(AttName2);
        remain_attribute.push_back(AttName3);
        remain_attribute.push_back(AttName4);
        remain_attribute.push_back(AttName5);

        vector<vector<string>>remain_state = vSamples;
        BuildingMap();
        root = DFS_building(root, remain_state, remain_attribute);
        cout << "the decision tree is :" << endl;
        PrintTree(root, 0);
        cout << endl;
    }
    void PredictProcess(TreeNode* p) {
        if (p->strAttribute.empty()) {
            cout << p->strArrivedValue << endl;
            return;
        }
        string s;
        cout << p->strAttribute << endl;
        cin >> s;
        for (int i = 0; i < p->vecTreeChild.size(); i++) {
            if (p->vecTreeChild[i]->strArrivedValue == s) {
                PredictProcess(p->vecTreeChild[i]);
            }
        }
    }
    void Predict() {
        PredictProcess(root);
    }
};
#endif