#include "open.h"
#include "utils.h"

Open::Open(Node* start) {
    nodes.push_back(start);
}

bool Open::isEmpty() {
    return nodes.size() == 0;
}

void Open::addSort(Node* node) {
    for (int i = 0, len = nodes.size(); i < len; i++) {
        if (node->f<nodes[i]->f) {
            //将node插在nodes[i]的前面，也就是第i个位置
            nodes.insert(nodes.begin() + i, node);
            return;
        }
    }
    nodes.push_back(node);//比所有都大，插在最后一个
}

//在open表中查找node
//若找到且f更小，replace
//找不到，add
void Open::findReplaceAdd(Node* node) {
    for (int i = 0, len = nodes.size(); i < len; i++) {
        if (same(nodes[i]->state, node->state,node->m,node->n)) {
            if (node->f < nodes[i]->f) {
                //将老的节点弹出，新的节点插入到原来位置
                nodes.erase(nodes.begin() + i);
                this->addSort(node);
                return;
            }
        }
    }
    addSort(node);//如果没有找到相同的节点，直接插入
}

Node* Open::popFirst() {
    Node* tmp = nodes[0];
    nodes.erase(nodes.begin());
    return tmp;
}
