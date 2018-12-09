#ifndef LOGIC_H
#define LOGIC_H

/*
 * 定义所有的逻辑，包括判断可行性，寻找解决路径
*/

#include <cmath>//计算曼哈顿距离
#include <vector>

using namespace std;

typedef enum{LEFT,RIGHT,UP,DOWN} Direction;

class Node{//每个可能的九宫格对应一个Node
public:
    int** matrix;//每个格子的数字
    int r,c;//空格所在位置
    int total,start2now,now2end;//三个启发函数
                                //start2now：开始状态到现在的距离，定义为搜索深度
                                //now2end:现在到目标的距离，定义为所有数字的曼哈顿距离之和
                                //total=start2now+now2end
     Node* father;//自己从哪个状态变化而来
     Node():matrix(nullptr){}
    Direction parentdir;//父亲节点为了到达自己，空格移动的方向
};

//open表中的节点
class OpenNode{
public:
    Node* node;
    int total;
    //当一个开节点变成闭节点，node=NULL,total=-1，既做了标记又不会出现node->total这样的越界访问错误
};

//求解问题
class Solver{
public:
    int m,n;//行，列的数量

    vector<int**> solve();//返回路径
private:
    int openCloseNum;//开节点和闭节点的总数
    int openNum;//开节点的总数
    Node Start,End;//开始节点和目标节点
    OpenNode open[10000];

    //通过用户设置初始化全局变量
    void setScale(int _m,int _n);
    void setNode(int** idx);
    int calNow2end(Node* node);//计算一个节点的nowsend
    bool isAble();//判断可行性
    vector<int**> path(Node* node);//从start到node的可行路径
    bool isEnd(Node* node);
    bool isEnd(int** idx);
    void sortOpen();//open表排序
    void move(Direction dir,Node* f);//父节点向子节点扩展
    void calFuncs(Node* f,Node* s);//根据父节点计算子节点的启发函数
    void addToOpen(Node* node);//将节点添加到open表
    void copyMatrix(Node* f,Node* s);//复制九宫格
    void extend(Node* node);//父节点向四个方向扩展
};


#endif // LOGIC_H
