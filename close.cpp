#include "close.h"
#include "utils.h"

Close::Close() {

}

void Close::add(Node* node) {
    nodes.push_back(node);
}

//将node按照f从小到大插入
void Close::addSort(Node* node) {
    for (int i = 0, len = nodes.size(); i < len; i++) {
        if (node->f < nodes[i]->f) {
            nodes.insert(nodes.begin() + i, node);
            return;
        }
    }
    nodes.push_back(node);
}

bool Close::find(Node* node) {
    for (int i = 0, len = nodes.size(); i < len; i++) {
        if (same(node->state, nodes[i]->state, node->m, node->n)) {
            if (node->depth < nodes[i]->depth) {
                nodes[i]->parent = node->parent;
            }
            return true;
        }
    }
    return false;
}

void Close::findReplaceAdd(Node* node) {
    for (int i = 0, len = nodes.size(); i < len; i++) {
        if (same(node->state, nodes[i]->state,node->m,node->n)) {
            if (node->f < nodes[i]->f) {
                nodes[i] = node;
                return;
            }
        }
    }
    addSort(node);
}
