//
// Created by Admin on 08.03.2016.
//

#include <iostream>
#include <map>
#include <chrono>
#include "stack.h"

using namespace std;

//Обход дерева в глубину
void depthTreeTraversal(nodeptr root ,int& count)
{
    if (root != NULL) {
        nodeptr leftnode = root->left;
        nodeptr rightnode = root->right;
        int thiscount = count;
        cout << root->element;
        if(root->parent != NULL)
            cout << ": parent = "  << root->parent->element;
        cout << " child: ";
        if(leftnode != NULL) {
            cout << leftnode->element << " ";
        }
        if(rightnode != NULL) {
            cout << rightnode->element << " ";
        }
        //cout << thiscount << " " << root->height;
        cout << endl;
        thiscount++;
        //insertToMap(node->data,s,summap);
        depthTreeTraversal(leftnode,thiscount);
        depthTreeTraversal(rightnode,thiscount);
    }
}
//Обход дерева в ширину
void wideTreeTraversal(nodeptr root, map<int,int> & summap)
{
    Stack *q = createStack();
    push(q, root);
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
        summap.insert(pair<int,int>(tmp->element, s));
        while(tmp != root)
        {
            tmp = tmp->parent;
            int v = summap.at(tmp->element);
            summap.erase(tmp->element);
            summap.insert(pair<int,int>(tmp->element,v + s));
        }
    }
    freeStack(&q);
}
int main()
{
    map<int,int> summap;
    nodeptr root = NULL;
    nodeptr parent = NULL;
    bstree tree;
    for(int i = 0; i < 100000; i++) {
        tree.insert(i, root,parent);
    }
    cout<<"height: "<<tree.bsheight(root)<<endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    wideTreeTraversal(root,summap);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time-start_time;
    printf("%d\n",summap.size());
    cout << endl;
    cout << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;
    return 0;
}