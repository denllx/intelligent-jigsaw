#include "node.h"
#include "solver.h"
#include "utils.h"
#include <algorithm>
#include <string.h>
#include <iostream>

using namespace std;

Node::Node(Node* parent, Solver* solver, string move) {
    num = solver->openNum;
    this->solver = solver;
    m = solver->m;
    n = solver->n;
    solver->openNum += 1;
    this->move=move;
    this->parent = parent;

    this->state = new int*[m];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            this->state[i] = new int[n];
    }

    if (this->move=="init") {//初始节点
        copy(this->state, solver->state0, m, n);
        this->depth = 0;
    }
    else {
        copy(this->state, parent->state, m, n);
        this->state = this->doMove(move);
        this->depth = parent->depth + 1;
    }
    this->f = calf();
    this->legalMoves = legalMove();//获取所有合法移动的位置
}

int** Node::doMove(string move) {
    if (solver->move_dict.find(move)!=solver->move_dict.end()){
        cout<<"move is in the dict";
    }
    else{
        cout<<"move is not in the dict";
    }
    pair<int, int> dir = this->solver->move_dict[move];
    //找到空格位置
    int r, c;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
        {
            if (state[i][j] == -1) {
                r = i;
                c = j;
            }
        }
    }
    int newr = r + dir.first;
    int newc = c + dir.second;//空格的新位置
    state[r][c] = state[newr][newc];
    state[newr][newc] = -1;
    return state;
}

int Node::manhatan() {
    int dis = 0;
    int x1, x2, y1, y2;
    for (int k = -1; k < m*n - 1; k++) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (state[i][j] == k) {
                    x1 = i;
                    y1 = j;
                }
                if (solver->stateFinal[i][j] == k) {
                    x2 = i;
                    y2 = j;
                }
            }
        }
        dis += abs(x1 - x2) + abs(y1 - y2);
    }
    return dis;
}

double Node::weightedMahatan() {
    double dis = 0;
    int x1, x2, y1, y2;
    for (int k = 0; k < m*n - 1; k++) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (state[i][j] == k) {
                    x1 = i;
                    y1 = j;
                }
                if (solver->stateFinal[i][j] == k) {
                    x2 = i;
                    y2 = j;
                }
            }
        }
        int increment = abs(x1 - x2) + abs(y1 - y2);
        if (x2 + y2 == 0)
            dis += increment * 1.5;
        else if (x2 + y2 <= 1)
            dis += increment * 1.3;
        else if (x2 + y2 <= 2)
            dis += increment * 1.1;
        else
            dis += increment;
    }
    return dis;
}

double Node::calf() {
    return weightedMahatan();
}

vector<string> Node::legalMove() {
    vector<string> ret{ "up","down","right","left" };
    int r, c;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (state[i][j] == -1) {
                r = i;
                c = j;
            }
        }
    }
    if (this->move!="init") {
        if (this->move=="up")
            ret.erase(std::remove(
                ret.begin(),
                ret.end(),
                "down"
            ), ret.end());
        else if (this->move=="down")
            ret.erase(std::remove(
                ret.begin(),
                ret.end(),
                "up"
            ), ret.end());
        else if (this->move=="left")
            ret.erase(std::remove(
                ret.begin(),
                ret.end(),
                "right"
            ), ret.end());
        else if (this->move=="right")
            ret.erase(std::remove(
                ret.begin(),
                ret.end(),
                "left"
            ), ret.end());
    }

    //是否处于边缘
    if (r==0)
        ret.erase(std::remove(
            ret.begin(),
            ret.end(),
            "up"
        ), ret.end());
    else if (r==m-1)
        ret.erase(std::remove(
            ret.begin(),
            ret.end(),
            "down"
        ), ret.end());
    if (c==0)
        ret.erase(std::remove(
            ret.begin(),
            ret.end(),
            "left"
        ), ret.end());
    else if (c==n-1)
        ret.erase(std::remove(
            ret.begin(),
            ret.end(),
            "right"
        ), ret.end());
    return ret;
}
