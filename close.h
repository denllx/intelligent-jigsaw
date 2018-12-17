#ifndef CLOSE_H
#define CLOSE_H
#include <vector>
#include "node.h"
using namespace std;

class Close {
public:
    Close();
    void add(Node* node);//无序插入
    void addSort(Node* node);//有序插入
    bool find(Node* node);
    void findReplaceAdd(Node* node);
private:
    vector<Node*> nodes;
};
#endif // CLOSE_H
