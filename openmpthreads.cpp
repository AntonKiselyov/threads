//
// Created by Admin on 21.03.2016.
//

#include <iostream>
#include <map>
#include <chrono>
#include <omp.h>
#include "stack.h"
#include <math.h>

using namespace std;

map<int,int>* pthread_map = new map<int,int>[8];
nodeptr* nodes;
map<int,int> * summap;
void insertToMap(int &n, int &s)
{
    summap->insert(pair<int,int>(n,s));
}
//Обход дерева в ширину, TASK
void wideTreeTraversalWithThreads(int &id) {
    Stack *q = createStack();
    Stack *parents = createStack();
    push(q, nodes[id]);
    push(parents, nodes[id]);
    while (q->size != 0) {
        nodeptr tmp;
        if( (tmp = (nodeptr) pop(q)) != NULL )
        {
            if( (tmp->parent != NULL ) && (tmp->parent != parents->data[parents->size-1]) && (pthread_map[id].count(parents->data[parents->size-1]->element) != 0 ) ){
                nodeptr eraseptr;
                do {
                    #pragma omp critical
                    {
                        insertToMap(parents->data[parents->size - 1]->element, pthread_map[id].at(parents->data[parents->size - 1]->element));
                    }
                    eraseptr = pop(parents);
                }while(tmp->parent != eraseptr->parent);
            }
        }

        int s = 0;
        if (tmp->left != NULL) {
            push(q, tmp->left);
            s += tmp->left->element;
            if( (tmp->left->left != NULL) || (tmp->left->right) != NULL)
                push(parents,tmp->left);
        }
        if (tmp->right != NULL) {
            push(q, tmp->right);
            s += tmp->right->element;
            if( (tmp->right->left != NULL) || (tmp->right->right) != NULL)
                push(parents,tmp->right);
        }
        pthread_map[id].insert(pair<int,int>(tmp->element, s));
        if(s == 0) {
            #pragma omp critical
            {
                insertToMap(tmp->element,s);
            }
        }
        while(tmp != nodes[id])
        {
            tmp = tmp->parent;
            int v = pthread_map[id].at(tmp->element);
            pthread_map[id].erase(tmp->element);
            pthread_map[id].insert(pair<int,int>(tmp->element,v + s));
        }
    }
    while(parents->size != 0) {
        #pragma omp critical
        {
            insertToMap(parents->data[parents->size - 1]->element,pthread_map[id].at(parents->data[parents->size - 1]->element));
        }
        pop(parents);
    }
    freeStack(&q);
    freeStack(&parents);
}

//Автоматизация распредления вершин
void auto_config(nodeptr bsroot, int level,int num, Stack* nodestack)
{
    if(level != log2(num)) {
        if(level == log2(num)-1) {
            push(nodestack, bsroot->left);
            push(nodestack, bsroot->right);
        }
        level++;
        auto_config(bsroot->left, level,num,nodestack);
        auto_config(bsroot->right, level,num,nodestack);
    }
}

int main(void) {
    int C = 100;
    nodeptr bsroot = NULL;
    nodeptr bsparent = NULL;
    bstree bstree;
    summap = new map<int, int>();
    for (int i = 0; i < 100000; i++) {
        bstree.insert(i, bsroot, bsparent);
    }
    cout << "height = " << bstree.bsheight(bsroot) << endl;
    int num = 8;
    nodes = new nodeptr[num];
    Stack *nodestack = createStack();//стек для вершин
    auto_config(bsroot, 0, num, nodestack);
    for (int i = 0; i < num; i++) {
        nodes[i] = (nodeptr) pop(nodestack);
    }
    freeStack(&nodestack);

    int sum8 = 0;
    long long  int* samples = new long long  int[C];
    for (int k = 0; k < C; k++) {
        auto start_time = std::chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(8)
        {
#pragma omp for
            for (int n = 0; n < num; ++n)
                wideTreeTraversalWithThreads(n);
        }

        int rootsum = bsroot->left->element + bsroot->right->element
                      + bsroot->left->left->element + bsroot->left->right->element
                      + bsroot->right->left->element + bsroot->right->right->element
                      + bsroot->left->left->left->element
                      + bsroot->left->left->right->element
                      + bsroot->left->right->left->element
                      + bsroot->left->right->right->element
                      + bsroot->right->left->left->element
                      + bsroot->right->left->right->element
                      + bsroot->right->right->left->element
                      + bsroot->right->right->right->element
                      + pthread_map[7].at(bsroot->left->left->left->element)
                      + pthread_map[6].at(bsroot->left->left->right->element)
                      + pthread_map[5].at(bsroot->left->right->left->element)
                      + pthread_map[4].at(bsroot->left->right->right->element)
                      + pthread_map[3].at(bsroot->right->left->left->element)
                      + pthread_map[2].at(bsroot->right->left->right->element)
                      + pthread_map[1].at(bsroot->right->right->left->element)
                      + pthread_map[0].at(bsroot->right->right->right->element);
        summap->insert(pair<int, int>(bsroot->element, rootsum));
        summap->insert(pair<int, int>(bsroot->left->element, bsroot->left->left->element
                                                             + bsroot->left->right->element
                                                             + bsroot->left->left->left->element
                                                             + bsroot->left->left->right->element
                                                             + pthread_map[7].at(bsroot->left->left->left->element)
                                                             + pthread_map[6].at(bsroot->left->left->right->element)
                                                             + bsroot->left->right->left->element
                                                             + bsroot->left->right->right->element
                                                             + pthread_map[5].at(bsroot->left->right->left->element)
                                                             + pthread_map[4].at(bsroot->left->right->right->element)));
        summap->insert(pair<int, int>(bsroot->right->element, bsroot->right->left->element
                                                              + bsroot->right->right->element
                                                              + bsroot->right->left->left->element
                                                              + bsroot->right->left->right->element
                                                              + pthread_map[3].at(bsroot->right->left->left->element)
                                                              + pthread_map[2].at(bsroot->right->left->right->element)
                                                              + bsroot->right->right->left->element
                                                              + bsroot->right->right->right->element
                                                              + pthread_map[1].at(bsroot->right->right->left->element)
                                                              +
                                                              pthread_map[0].at(bsroot->right->right->right->element)));
        summap->insert(pair<int, int>(bsroot->left->left->element,
                                      bsroot->left->left->left->element
                                      + bsroot->left->left->right->element
                                      + pthread_map[7].at(bsroot->left->left->left->element)
                                      + pthread_map[6].at(bsroot->left->left->right->element)));
        summap->insert(pair<int, int>(bsroot->left->right->element,
                                      bsroot->left->right->left->element
                                      + bsroot->left->right->right->element
                                      + pthread_map[5].at(bsroot->left->right->left->element)
                                      + pthread_map[4].at(bsroot->left->right->right->element)));
        summap->insert(pair<int, int>(bsroot->right->left->element,
                                      bsroot->right->left->left->element
                                      + bsroot->right->left->right->element
                                      + pthread_map[3].at(bsroot->right->left->left->element)
                                      + pthread_map[2].at(bsroot->right->left->right->element)));
        summap->insert(pair<int, int>(bsroot->right->right->element,
                                      bsroot->right->right->left->element
                                      + bsroot->right->right->right->element
                                      + pthread_map[1].at(bsroot->right->right->left->element)
                                      + pthread_map[0].at(bsroot->right->right->right->element)));


        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        sum8 += std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        samples[k]  = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        for (int i = 0; i < 8; i++)
            pthread_map[i].clear();
    }
    cout << endl;
    cout << endl << "Parallel time for 8 threads: " << sum8 / C << endl;
    long long  int dispersia = 0;
    for(int k = 0; k < C; k++) {
        samples[k] = pow((samples[k] - sum8 / C), 2);
        dispersia += samples[k];
    }
    delete[](samples);
    cout << endl << "Parallel dispersia for 8 threads: " << sqrt(dispersia/C) << endl;
    cout << summap->size() << endl;
    summap->clear();
    for (int i = 0; i < 8; i++)
        pthread_map[i].clear();

    num = 4;
    nodes = new nodeptr[num];
    nodestack = createStack();//стек для вершин
    auto_config(bsroot, 0, num, nodestack);
    for (int i = 0; i < num; i++) {
        nodes[i] = (nodeptr) pop(nodestack);
    }
    freeStack(&nodestack);


    int sum4 = 0;
    samples = new long long  int[C];
    for (int k = 0; k < C; k++) {
        auto start_time = std::chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(4)
        {
#pragma omp for
            for (int n = 0; n < num; ++n)
                wideTreeTraversalWithThreads(n);
        }

        int rootsum = bsroot->left->element + bsroot->right->element
                      + bsroot->left->left->element
                      + bsroot->left->right->element
                      + bsroot->right->left->element
                      + bsroot->right->right->element
                      + pthread_map[3].at(bsroot->left->left->element)
                      + pthread_map[2].at(bsroot->left->right->element)
                      + pthread_map[1].at(bsroot->right->left->element)
                      + pthread_map[0].at(bsroot->right->right->element);
        summap->insert(pair<int, int>(bsroot->element, rootsum));
        summap->insert(pair<int, int>(bsroot->left->element,
                                      bsroot->left->left->element + bsroot->left->right->element +
                                      pthread_map[3].at(bsroot->left->left->element)
                                      + pthread_map[2].at(bsroot->left->right->element)));
        summap->insert(pair<int, int>(bsroot->right->element,
                                      bsroot->right->left->element + bsroot->right->right->element +
                                      pthread_map[1].at(bsroot->right->left->element)
                                      + pthread_map[0].at(bsroot->right->right->element)));
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        sum4 += std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        samples[k] = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        for (int i = 0; i < 8; i++)
            pthread_map[i].clear();
    }
    cout << endl;
    cout << endl << "Parallel time for 4 threads: " << sum4 / C << endl;
    cout << summap->size() << endl;
    dispersia = 0;
    for(int k = 0; k < C; k++) {
        samples[k] = pow((samples[k] - sum4 / C), 2);
        dispersia += samples[k];
    }
    delete[](samples);
    cout << endl << "Parallel dispersia 4 threads: " << sqrt(dispersia/C) << endl;
    summap->clear();
    for (int i = 0; i < 8; i++)
        pthread_map[i].clear();

    num = 2;
    nodes = new nodeptr[num];
    nodestack = createStack();//стек для вершин
    auto_config(bsroot, 0, num, nodestack);
    for (int i = 0; i < num; i++) {
        nodes[i] = (nodeptr) pop(nodestack);
    }
    freeStack(&nodestack);

    int sum2 = 0;
    samples = new long long  int[C];
    for (int k = 0; k < C; k++) {
        auto start_time = std::chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(2)
        {
#pragma omp for
            for (int n = 0; n < num; ++n)
                wideTreeTraversalWithThreads(n);
        }
        int rootsum = bsroot->left->element + bsroot->right->element + pthread_map[1].at(bsroot->left->element)
                      + pthread_map[0].at(bsroot->right->element);
        summap->insert(pair<int, int>(bsroot->element, rootsum));
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        sum2 += std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        samples[k] = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        for (int i = 0; i < 8; i++)
            pthread_map[i].clear();
    }
    cout << endl;
    cout << endl << "Parallel time for 2 threads: " << sum2 / C << endl;
    cout << summap->size() << endl;
    dispersia = 0;
    for(int k = 0; k < C; k++) {
        samples[k] = pow((samples[k] - sum2 / C), 2);
        dispersia += samples[k];
    }
    delete[](samples);
    cout << endl << "Parallel dispersia 2 threads: " << sqrt(dispersia/C) << endl;
    summap->clear();
    for (int i = 0; i < 8; i++)
        pthread_map[i].clear();


    num = 1;
    nodes[0] = bsroot;
    int sum1 = 0;
    samples = new long long  int[C];
    for (int k = 0; k < C; k++) {
        auto start_time = std::chrono::high_resolution_clock::now();
    #pragma omp parallel num_threads(1)
        {
    #pragma omp for
            for (int n = 0; n < num; ++n)
                wideTreeTraversalWithThreads(n);
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        sum1+=std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        samples[k] = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        for (int i = 0; i < 8; i++)
            pthread_map[i].clear();
    }
    cout << endl;
    cout << endl << "Sequence time: " << sum1/C << endl;
    cout << summap->size() << endl;
    dispersia = 0;
    for(int k = 0; k < C; k++) {
        samples[k] = pow((samples[k] - sum1 / C), 2);
        dispersia += samples[k];
    }
    delete[](samples);
    cout << endl << "Sequence dispersias: " << sqrt(dispersia/C) << endl;

    delete(summap);
    for(int i = 0; i < 8; i++)
        delete(&pthread_map[i]);
    return 0;
}