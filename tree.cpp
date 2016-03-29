//
// Created by Admin on 08.03.2016.
//

#include <iostream>
#include <map>
#include <chrono>
#include "stack.h"

using namespace std;

//Тест обхода в глубину, результаты обработки теста должны соответствовать массиву значений , полученным при обходе в ширину 
void test1() 
{
    nodeptr root = NULL;
    nodeptr parent = NULL;
    bstree tree;
    for(int i = 0; i < 100000; i++) {
        tree.insert(i, root,parent);
    }
    map<int,int> modelmap;
    map<int,int> testmap;
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

//Добавление в карту
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
        cout << "test2 success" << endl;
    }
    else
    {
        cout << "test2 failed" << endl;
    }
    testmap.clear();
}


nodeptr mainroot = NULL;
//Обход дерева в глубину
void depthTreeTraversal(nodeptr root ,map<int,int> &summap)
{
    if(mainroot == NULL)
        mainroot = root;
    if (root != NULL) {
        nodeptr leftnode = root->left;
        nodeptr rightnode = root->right;
    //    cout << root->element;
    //    if(root->parent != NULL)
      //      cout << ": parent = "  << root->parent->element;
     //   cout << " child: ";
        int s = 0;
        if(leftnode != NULL) {
     //       cout << leftnode->element << " ";
            s += leftnode->element;

        }
        if(rightnode != NULL) {
     //       cout << rightnode->element << " ";
            s += rightnode->element;
        }
      //  cout << root->height;
     //   cout << endl;
        insertToMap(root->element,s,summap);
        while(root != mainroot)
        {
            root = root->parent;
            int v = summap.at(root->element);
            summap.erase(root->element);
            summap.insert(pair<int,int>(root->element,v + s));
        }
        depthTreeTraversal(leftnode,summap);
        depthTreeTraversal(rightnode,summap);
    }
}
//Обход дерева в ширину
void wideTreeTraversal(nodeptr root, map<int,int> & summap)
{
    Stack *q = createStack();
    push(q, root);
    int reversesum = 0;
    while (q->size != 0) {
        nodeptr tmp = q->data[q->size-1];
        if( ((tmp->left != NULL) && (summap.count(tmp->left->element) != 0) ) || ( (tmp->right != NULL) && (summap.count(tmp->right->element) != 0))   ) {
            int value = summap.at(tmp->element);
            summap.erase(tmp->element);
            summap.insert(pair<int, int>(tmp->element, value + reversesum));
            reversesum += value;
            pop(q);
        } else {
            if(summap.count(q->data[q->size-2]->element) != 0) {
                int value = summap.at(q->data[q->size-2]->element);
                summap.erase(q->data[q->size-2]->element);
                summap.insert(pair<int, int>(q->data[q->size-2]->element, value + reversesum));
            }
            reversesum = 0;
            int s = 0;
            if (tmp->left != NULL) {
                push(q, tmp->left);
                s += tmp->left->element;
            }
            if (tmp->right != NULL) {
                push(q, tmp->right);
                s += tmp->right->element;
            }
            summap.insert(pair<int, int>(tmp->element, s));
            if (s == 0) {
                pop(q);
            }
        }
    }
    freeStack(&q);
}

int main()
{
   // test1();
   // test2();
    map<int,int> summap;
    map<int,int> etalonmap;
    nodeptr root = NULL;
    nodeptr parent = NULL;
    bstree tree;
    for(int i = 0; i < 100000; i++) {
        tree.insert(i, root,parent);
    }
    cout<<"height: "<<tree.bsheight(root)<<endl;
    depthTreeTraversal(root,etalonmap);
    auto start_time = std::chrono::high_resolution_clock::now();
    wideTreeTraversal(root,summap);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time-start_time;
    printf("%d\n",summap.size());
    printf("%d\n",etalonmap.size());
    cout << endl;
    cout << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;
    return 0;
}