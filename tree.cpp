//
// Created by Admin on 23.02.2016.
//


/* red-black tree */

#include <iostream>
#include <map>
#include <chrono>
#include "stack.h"
#include <time.h>
using namespace std;

void depthTreeTraversal(Node* node, map<int,int> &summap);
void wideTreeTraversal(Node* root, map<int,int> & summap);

//Тест обхода в глубину, результаты обработки теста должны соответствовать массиву значений , полученным при обходе в ширину
void test1()
{
    map<int,int> modelmap;
    map<int,int> testmap;
    for(int i = 25; i >= 0; i--)
        insertNode(i);
    wideTreeTraversal(root,modelmap);
    depthTreeTraversal(root,testmap);
    if(modelmap == testmap)
    {
        cout << "test1 success" << endl;
    } else {
        cout << "test1 failed" << endl;
    }
    modelmap.clear();
    testmap.clear();
}

void insertToMap(int &n, int &s, map<int,int> &summap)
{
    summap.insert(pair<int,int>(n,s));
}
//Тест на проверку вставки в map
void test2()
{
    map<int,int> testmap;
    int key = 1;
    int value = 3;
    insertToMap(key,value,testmap);
    if(testmap.find(1)->second == 3)
    {
        cout << "test2 success"  << endl;
    }
    else
    {
        cout << "test2 failed" << endl;
    }
    testmap.clear();
}

//Обход дерева в глубину
void depthTreeTraversal(Node* node, map<int,int> &summap)
{
    if (node != NIL) {
        Node* leftnode = node->left;
        Node* rightnode = node->right;
        int s = 0;
        if(leftnode != NIL) {
            s += leftnode->data;
        }
        if(rightnode != NIL) {
            s += rightnode->data;
        }
        insertToMap(node->data,s,summap);
        depthTreeTraversal(leftnode,summap);
        depthTreeTraversal(rightnode,summap);
    }
}


//Обход дерева в ширину
void wideTreeTraversal(Node* root, map<int,int> & summap) {
    Stack *q = createStack();
    push(q, root);
    while (q->size != 0) {
        Node *tmp = (Node*) pop(q);
        int s = 0;
        if (tmp->left != NIL) {
            push(q, tmp->left);
            s += tmp->left->data;
        }
        if (tmp->right != NIL) {
            push(q, tmp->right);
            s += tmp->right->data;
        }
        summap.insert(pair<int,int>(tmp->data, s));
    }
    freeStack(&q);
}

int main(int argc, char **argv) {
    //test1();
  //  test2();
    for(int i = 10025; i >= 0; i--)
        insertNode(i);
    map<int,int> summap;
    auto start_time = std::chrono::high_resolution_clock::now();
    depthTreeTraversal(root,summap);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto time = end_time-start_time;

    cout << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;
    return 0;
}