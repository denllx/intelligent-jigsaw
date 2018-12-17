#ifndef OPEN_H
#define OPEN_H

#include "node.h"

class Open {
public:
    Open(Node* start);
    bool isEmpty();
    void addSort(Node* node);//添加并排序
    void findReplaceAdd(Node* node);
    Node* popFirst();//将open表第一个节点弹出
private:
    vector<Node*> nodes;//存放开节点
};

#endif // OPEN_H
