#ifndef SOLVER_H
#define SOLVER_H
#include <iostream>
#include <map>
#include "node.h"
#include <vector>
#include "open.h"
#include "close.h"

using namespace std;

typedef pair<int, int> Direction;

class Solver {
public:
    int m, n;
    int openNum=0;//开节点总数
    map<string, Direction> move_dict;
    Open* open;
    Close* close;

    Solver(int _m,int n_,int** idx);
    vector<int**> solve();
    void test();
    bool isAble();
    void initstart(int** idx);//输入state0
    void initend();//规定statefinal
    void initDict();//初始化move_dict
    int** state0, **stateFinal;//初始和目标状态矩阵
    vector<int**> path(Node* node);//从node回溯路径
private:


};
#endif // SOLVER_H
