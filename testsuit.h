//
// Created by Admin on 24.02.2016.
//

#ifndef THREADS_TESTSUIT_H
#define THREADS_TESTSUIT_H

#include <cxxtest-4.3/cxxtest/TestSuite.h>
#include "tree.h"
class Suit :: CxxTest::TestSuit
{
public:

//Тест обхода в глубину, результаты обработки теста должны соответствовать массиву значений , полученным при обходе в ширину
    void test1()
    {
        map<int,int> modelmap;
        map<int,int> testmap;
        for(int i = 25; i >= 0; i--)
            insertNode(i);
        wideTreeTraversal(root,modelmap);
        depthTreeTraversal(root,testmap);
        modelmap.clear();
        testmap.clear();
    }
};


#endif //THREADS_TESTSUIT_H
