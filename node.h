#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>

using namespace std;

class Solver;

class Node {
private:
        string move;//父亲通过哪个方向达到自己
        Solver* solver;

        int** doMove(string move);//空格移动一次
        vector<string> legalMove();//合法的移动
        int manhatan();//计算到目标节点的曼哈顿距离
        double weightedMahatan();//带权重的曼哈顿距离
        double calf();//计算损失函数f
public:
        int m, n;
        int num;//自己处在开节点表中的第几个
        int depth;//搜索深度
        double f;//损失函数
        int** state;//状态矩阵
        Node* parent;//父亲节点
        vector<string> legalMoves;//所有可以移动的方向

        Node(Node* parent, Solver* solver, string move);
};
#endif
