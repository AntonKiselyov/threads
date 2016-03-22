//
// Created by Admin on 21.03.2016.
//

#include <iostream>
#include <map>
#include "stack.h"
using namespace std;
map<int,int>* pthread_map = new map<int,int>[8];

nodeptr nodes[2];

struct arguments {
    int id;
    nodeptr root;
};

//Обход дерева в ширину, TASK
void wideTreeTraversalWithThreads(int &id) {
    int count = 0;
    Stack *q = createStack();
    push(q, nodes[id]);
    while (q->size != 0) {
        nodeptr tmp = (nodeptr) pop(q);
        int s = 0;
        if (tmp->left != NULL) {
            push(q, tmp->left);
            s += tmp->left->element;
        }
        if (tmp->right != NULL) {
            push(q, tmp->right);
            s += tmp->right->element;
        }
        pthread_map[id].insert(pair<int,int>(tmp->element, s));
        while(tmp != nodes[id])
        {
            tmp = tmp->parent;
            int v = pthread_map[id].at(tmp->element);
            pthread_map[id].erase(tmp->element);
            pthread_map[id].insert(pair<int,int>(tmp->element,v + s));
        }
        count++;
    }
    freeStack(&q);
}

int main(void)
{
    nodeptr bsroot = NULL;
    nodeptr bsparent = NULL;
    bstree bstree;
    for(int i = 0; i < 100000; i++) {
        bstree.insert(i, bsroot,bsparent);
    }
    cout << "height = " << bstree.bsheight(bsroot) << endl;

    nodes[0] = bsroot->left->left->left;
    nodes[1] = bsroot->left->left->right;
    nodes[2] = bsroot->left->right->left;
    nodes[3] = bsroot->left->right->right;
    nodes[4] = bsroot->right->left->left;
    nodes[5] = bsroot->right->left->right;
    nodes[6] = bsroot->right->right->left;
    nodes[7] = bsroot->right->right->right;
    #pragma omp parallel num_threads(2)
    {
        #pragma omp for
        for(int n = 0; n < 8; ++n)
            wideTreeTraversalWithThreads(n);
    }
    for(int i = 0; i < 8; i++)
        printf("Node #%i, %i\n",i,pthread_map[i].size());

    return 0;
}