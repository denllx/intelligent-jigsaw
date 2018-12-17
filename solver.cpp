#include "solver.h"
#include <time.h>
#include <iostream>
using namespace std;
void Solver::test() {
    cout << move_dict["up"].first << "," << move_dict["up"].second << endl;
}
vector<int **> Solver::solve() {
    clock_t start = clock();
    clock_t end;

    Node* s0 = new Node(NULL, this, "init");
    open = new Open(s0);
    close = new Close();
    while (!open->isEmpty()) {
        Node* curNode = open->popFirst();//首节点
        //cout << "cur node num:" << curNode->num << " d:" << curNode->depth << " f:" << curNode->f << endl;
        /*for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++)
                cout << curNode->state[i][j] << " ";
            cout << endl;
        }*/
        close->add(curNode);
        if (curNode->f == 0) {
            cout << "success!" << endl;
            end = clock();
            vector<int**> ret = path(curNode);
            cout << "time cost:" << double(double((end - start)) / CLOCKS_PER_SEC) << "seconds" << endl;
            return ret;
        }
        else {
            //扩展节点
            for (auto move : curNode->legalMoves) {
                cout<<"move:"<<move<<endl;
                Node* n = new Node(curNode, this, move);
                if (!close->find(n)) {//如果不在close表中
                    open->findReplaceAdd(n);
                }
            }
        }
    }
    return vector<int**>(0);
}

void Solver::initDict() {
    move_dict["up"] = pair<int, int>(-1, 0);
    move_dict["down"] = pair<int, int>(1, 0);
    move_dict["right"] = pair<int, int>(0, 1);
    move_dict["left"] = pair<int, int>(0, -1);
}

Solver::Solver(int _m, int _n, int **idx) {
    m = _m;
    n = _n;
    initstart(idx);
    initend();
    initDict();
}
vector<int**> Solver::path(Node* node) {
    vector<int**> ret;
    ret.push_back(node->state);
    while (node->parent) {
        ret.insert(ret.begin(), node->parent->state);
        node = node->parent;
    }
    for (int idx = 0, len = ret.size(); idx < len;idx++) {
        cout << "Step:" << idx << endl;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++)
                cout << ret[idx][i][j] << " ";
        }
    }
    return ret;
}

void Solver::initstart(int **idx) {
    state0 = new int*[m];
    for (int i = 0; i < m; i++) {
        state0[i] = new int[n];
        for (int j = 0; j < n; j++)
            state0[i][j]=idx[i][j];
    }
}

bool Solver::isAble(){
    int reverse=0,k=0;
    int* s=new int[m*n];
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++)
        s[k++]=state0[i][j];
    }
    int x=-1,y=-1;
    for (int i=0;i<m*n;i++){
        int a=s[i];
        if (a==-1){
            x=i/n;
            y=i%n;
        }
        for (int j=i+1;j<m*n;j++){
            if (s[j]<a) reverse++;
        }
    }
    delete s;
    int sum1=reverse+x+y;
    int sum2=m*n-1+m+n-2;
    return (sum1%2==sum2%2);
}

void Solver::initend() {
    stateFinal = new int*[m];
    for (int i = 0; i < m; i++) {
        stateFinal[i] = new int[n];
        for (int j = 0; j < n; j++) {
            stateFinal[i][j] = i * n + j;
        }
    }
    stateFinal[m - 1][n - 1] = -1;
}
