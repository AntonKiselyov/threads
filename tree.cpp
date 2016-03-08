//
// Created by Admin on 23.02.2016.
//


/* red-black tree */

#include <iostream>
#include <map>
#include <chrono>
#include "stack.h"
#include <queue>
using namespace std;

void depthTreeTraversal(Node* node, map<int,int> &summap,int& count);
void wideTreeTraversal(Node* root, map<int,int> & summap);

//Тест обхода в глубину, результаты обработки теста должны соответствовать массиву значений , полученным при обходе в ширину
void test1()
{
    map<int,int> modelmap;
    map<int,int> testmap;
    for(int i = 25; i >= 0; i--)
        insertNode(i);
    wideTreeTraversal(root,modelmap);
//    depthTreeTraversal(root,testmap);
    if(modelmap == testmap)
    {
        cout << "test1 success" << endl;
    } else {
        cout << "test1 failed" << endl;
    }
    modelmap.clear();
    testmap.clear();
}
int *arr = new int[1000000];
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
void depthTreeTraversal(Node* node, map<int,int> &summap,int& count)
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
        int thiscount = count;
        cout << node->data << ": " << node->left->data << " " << node->right->data << ": " << thiscount << endl;
        thiscount++;
        //insertToMap(node->data,s,summap);
        depthTreeTraversal(leftnode,summap,thiscount);
        depthTreeTraversal(rightnode,summap,thiscount);
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
        //arr[tmp->data,s];
        summap.insert(pair<int,int>(tmp->data, s));
    }
    freeStack(&q);
}

int main(int argc, char **argv) {
    //test1();
  //  test2();
    for(int i = 1000000; i >= 0; i--)
        insertNode(i);
    map<int,int> summap;
    int count  = 0;
  //  depthTreeTraversal(root,summap,count);
    auto start_time = std::chrono::high_resolution_clock::now();
    wideTreeTraversal(root,summap);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto time = end_time-start_time;

    cout << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

    cout << summap.size() << endl;

    return 0;
}