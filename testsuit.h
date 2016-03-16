//
// Created by Admin on 24.02.2016.
//

#ifndef THREADS_TESTSUIT_H
#define THREADS_TESTSUIT_H

#include <cxxtest-4.3/cxxtest/TestSuite.h>
#include "stack.h"
class Suit :: CxxTest::TestSuit
{
public:

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
};


#endif //THREADS_TESTSUIT_H
