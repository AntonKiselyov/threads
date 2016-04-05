//
// Created by Admin on 01.03.2016.
//

#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <windows.h>
#include <sys/time.h>
#include <unistd.h>
#include <map>
#include <math.h>
#include "avl.h"
#include "stack.h"
using namespace std;

pthread_mutex_t mutex;
CRITICAL_SECTION CriticalSection;
map<int,int>* summap;
map<int,int>* pthread_map = new map<int,int>[8];

struct arguments {
    int id;
    nodeptr root;
};

int countWorkThreads;//Число работающих потоков
int countEndThreads;//Число законченных потоков

void* wideTreeTraversalWithThreads(void* arg);

//Тесты для параллельной обработки
void test4threads(nodeptr bsroot)
{
    pthread_t thread[8];
    int status[8];
    int status_addr[8];
    int threadid[8];
    countEndThreads = 0;
    countWorkThreads = 0;
    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();

    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->left->left;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 1) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->left->right;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 2) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->right->left;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 3) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->right->right;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        countWorkThreads++;
        pthread_detach(thread[i]);
    }
    while(countEndThreads < countWorkThreads)
        usleep(1);

    map<int,int> testmap;
    int rootsum = bsroot->left->element + bsroot->right->element
                  + bsroot->left->left->element
                  + bsroot->left->right->element
                  + bsroot->right->left->element
                  + bsroot->right->right->element
                  + pthread_map[0].at(bsroot->left->left->element)
                  + pthread_map[1].at(bsroot->left->right->element)
                  + pthread_map[2].at(bsroot->right->left->element)
                  + pthread_map[3].at(bsroot->right->right->element);
    testmap.insert(pthread_map[0].begin(),pthread_map[0].end());
    testmap.insert(pthread_map[1].begin(),pthread_map[1].end());
    testmap.insert(pthread_map[2].begin(),pthread_map[2].end());
    testmap.insert(pthread_map[3].begin(),pthread_map[3].end());
    testmap.insert(pair<int,int> (bsroot->element,rootsum));
    testmap.insert(pair<int,int> (bsroot->left->element,bsroot->left->left->element + bsroot->left->right->element + pthread_map[0].at(bsroot->left->left->element)
                                                        + pthread_map[1].at(bsroot->left->right->element)));
    testmap.insert(pair<int,int> (bsroot->right->element,bsroot->right->left->element + bsroot->right->right->element + pthread_map[2].at(bsroot->right->left->element)
                                                         + pthread_map[3].at(bsroot->right->right->element)));

    if(testmap == *summap)
        cout << "test 4 success" << endl;
    else
        cout << "test 4 failed" << endl;
}

void test8threads(nodeptr bsroot)
{
    pthread_t thread[8];
    int status[8];
    int status_addr[8];
    int threadid[8];
    countEndThreads = 0;
    countWorkThreads = 0;
    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();
    for (int i = 0; i < 8; i++) {
        if (i == 0) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->left->left->left;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 1) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->left->left->right;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 2) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->left->right->left;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 3) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->left->right->right;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 4) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->right->left->left;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 5) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->right->left->right;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 6) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->right->right->left;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 7) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->right->right->right;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        countWorkThreads++;
        pthread_detach(thread[i]);
    }
    while(countEndThreads < countWorkThreads)
        usleep(1);

    map<int,int> testmap;
    testmap.insert(pthread_map[0].begin(),pthread_map[0].end());
    testmap.insert(pthread_map[1].begin(),pthread_map[1].end());
    testmap.insert(pthread_map[2].begin(),pthread_map[2].end());
    testmap.insert(pthread_map[3].begin(),pthread_map[3].end());
    testmap.insert(pthread_map[4].begin(),pthread_map[4].end());
    testmap.insert(pthread_map[5].begin(),pthread_map[5].end());
    testmap.insert(pthread_map[6].begin(),pthread_map[6].end());
    testmap.insert(pthread_map[7].begin(),pthread_map[7].end());
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
              + pthread_map[0].at(bsroot->left->left->left->element)
              + pthread_map[1].at(bsroot->left->left->right->element)
              + pthread_map[2].at(bsroot->left->right->left->element)
              + pthread_map[3].at(bsroot->left->right->right->element)
              + pthread_map[4].at(bsroot->right->left->left->element)
              + pthread_map[5].at(bsroot->right->left->right->element)
              + pthread_map[6].at(bsroot->right->right->left->element)
              + pthread_map[7].at(bsroot->right->right->right->element);
    testmap.insert(pair<int,int> (bsroot->element,rootsum));
    testmap.insert(pair<int,int> (bsroot->left->element,bsroot->left->left->element
                                                        + bsroot->left->right->element
                                                        + bsroot->left->left->left->element
                                                          + bsroot->left->left->right->element
                                                          + pthread_map[0].at(bsroot->left->left->left->element)
                                                          + pthread_map[1].at(bsroot->left->left->right->element)
                                                        + bsroot->left->right->left->element
                                                          + bsroot->left->right->right->element
                                                          + pthread_map[2].at(bsroot->left->right->left->element)
                                                          + pthread_map[3].at(bsroot->left->right->right->element)));
    testmap.insert(pair<int,int> (bsroot->right->element,bsroot->right->left->element
                                                         + bsroot->right->right->element
                                                         + bsroot->right->left->left->element
                                                           + bsroot->right->left->right->element
                                                           + pthread_map[4].at(bsroot->right->left->left->element)
                                                           + pthread_map[5].at(bsroot->right->left->right->element)
                                                         + bsroot->right->right->left->element
                                                           + bsroot->right->right->right->element
                                                           + pthread_map[6].at(bsroot->right->right->left->element)
                                                           + pthread_map[7].at(bsroot->right->right->right->element)));
    testmap.insert(pair<int,int> (bsroot->left->left->element,
                                bsroot->left->left->left->element
                                + bsroot->left->left->right->element
                                + pthread_map[0].at(bsroot->left->left->left->element)
                                + pthread_map[1].at(bsroot->left->left->right->element)));
    testmap.insert(pair<int,int> (bsroot->left->right->element,
                                 bsroot->left->right->left->element
                                 + bsroot->left->right->right->element
                                 + pthread_map[2].at(bsroot->left->right->left->element)
                                 + pthread_map[3].at(bsroot->left->right->right->element)));
    testmap.insert(pair<int,int> (bsroot->right->left->element,
                                  bsroot->right->left->left->element
                                + bsroot->right->left->right->element
                                + pthread_map[4].at(bsroot->right->left->left->element)
                                + pthread_map[5].at(bsroot->right->left->right->element)));
    testmap.insert(pair<int,int> (bsroot->right->right->element,
                                  bsroot->right->right->left->element
                                 + bsroot->right->right->right->element
                                 + pthread_map[6].at(bsroot->right->right->left->element)
                                 + pthread_map[7].at(bsroot->right->right->right->element)));
    if(testmap == *summap)
        cout << "test 8 success" << endl;
    else
        cout << "test 8 failed" << endl;
}

void test2threads(nodeptr bsroot)
{
    pthread_t thread[8];
    int status[8];
    int status_addr[8];
    int threadid[8];
    countEndThreads = 0;
    countWorkThreads = 0;
    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();

    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->left;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        else if (i == 1) {
            arguments* arg = new arguments;
            arg->id = i;
            arg->root = bsroot->right;
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                printf("Can't create thread!\n");
            }
        }
        countWorkThreads++;
        pthread_detach(thread[i]);
    }
    while(countEndThreads < countWorkThreads)
        usleep(1);
    map<int,int> testmap;
    testmap.insert(pthread_map[0].begin(),pthread_map[0].end());
    testmap.insert(pthread_map[1].begin(),pthread_map[1].end());
    int rootsum = bsroot->left->element + bsroot->right->element + pthread_map[0].at(bsroot->left->element)
              + pthread_map[1].at(bsroot->right->element);
    testmap.insert(pair<int,int> (bsroot->element,rootsum));
    if(testmap == *summap)
        cout << "test 2 success" << endl;
    else
        cout << "test 2 failed" << endl;
}



void insertToMap(int &n, int s)
{

    summap->insert(pair<int,int>(n,s));
    
}
/*
//Обход дерева в ширину
void* wideTreeTraversalWithThreads(void* arg) {
    Stack *q = createStack();
    Stack *parents = createStack();
    push(q,( (arguments*)arg )->root);
    push(parents,( (arguments*)arg )->root);
    while (q->size != 0) {
        nodeptr tmp;
        if( (tmp = (nodeptr) pop(q)) != NULL )
        {
            if( (tmp->parent != NULL ) && (tmp->parent != parents->data[parents->size-1]) && (pthread_map[( (arguments*)arg )->id].count(parents->data[parents->size-1]->element) != 0 ) ){
                nodeptr eraseptr;
                 do {
                    pthread_mutex_lock(&mutex);
                    insertToMap(parents->data[parents->size - 1]->element,
                                pthread_map[((arguments *) arg)->id].at(parents->data[parents->size - 1]->element));
                    pthread_mutex_unlock(&mutex);
/*EnterCriticalSection(&CriticalSection);
insertToMap(parents->data[parents->size - 1]->element,
            pthread_map[((arguments *) arg)->id].at(parents->data[parents->size - 1]->element));
LeaveCriticalSection(&CriticalSection);*//*
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
        pthread_map[( (arguments*)arg )->id].insert(pair<int,int>(tmp->element, s));
        if(s == 0) {
            pthread_mutex_lock(&mutex);
            insertToMap(tmp->element,s);
            pthread_mutex_unlock(&mutex);*/
           /* EnterCriticalSection(&CriticalSection);
            insertToMap(tmp->element,s);
            LeaveCriticalSection(&CriticalSection);*//*
        }
        while(tmp != ( (arguments*)arg )->root)
        {
            tmp = tmp->parent;
            int v = pthread_map[( (arguments*)arg )->id].at(tmp->element);
            pthread_map[( (arguments*)arg )->id].erase(tmp->element);
            pthread_map[( (arguments*)arg )->id].insert(pair<int,int>(tmp->element,v + s));
        }
    }
    while(parents->size != 0) {
        pthread_mutex_lock(&mutex);
        insertToMap(parents->data[parents->size - 1]->element,
                    pthread_map[((arguments *) arg)->id].at(parents->data[parents->size - 1]->element));
        pthread_mutex_unlock(&mutex);
        /*EnterCriticalSection(&CriticalSection);
        insertToMap(parents->data[parents->size - 1]->element,
                    pthread_map[((arguments *) arg)->id].at(parents->data[parents->size - 1]->element));
        LeaveCriticalSection(&CriticalSection);*//*
        pop(parents);
    }
    freeStack(&q);
    freeStack(&parents);
    pthread_mutex_lock(&mutex);
    countEndThreads++;
    pthread_mutex_unlock(&mutex);
}*/

//Обход дерева в ширину
void* wideTreeTraversalWithThreads(void* arg) {
    Stack *q = createStack();
    push(q,( (arguments*)arg )->root);
    int reversesum = 0;
    while (q->size != 0) {
        nodeptr tmp = q->data[q->size - 1];
        if (((tmp->left != NULL) && (pthread_map[((arguments *) arg)->id].count(tmp->left->element) != 0)) ||
            ((tmp->right != NULL) && (pthread_map[((arguments *) arg)->id].count(tmp->right->element) != 0))) {
            int value = pthread_map[((arguments *) arg)->id].at(tmp->element);
            pthread_map[((arguments *) arg)->id].erase(tmp->element);
            pthread_map[((arguments *) arg)->id].insert(pair<int, int>(tmp->element, value + reversesum));
            pthread_mutex_lock(&mutex);
            insertToMap(tmp->element,pthread_map[((arguments *) arg)->id].at(tmp->element));
            pthread_mutex_unlock(&mutex);
            reversesum += value;
            pop(q);

        } else {
            if (pthread_map[((arguments *) arg)->id].count(q->data[q->size - 2]->element) != 0) {
                int value = pthread_map[((arguments *) arg)->id].at(q->data[q->size - 2]->element);
                pthread_map[((arguments *) arg)->id].erase(q->data[q->size - 2]->element);
                pthread_map[((arguments *) arg)->id].insert(
                        pair<int, int>(q->data[q->size - 2]->element, value + reversesum));
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
            pthread_map[((arguments *) arg)->id].insert(pair<int, int>(tmp->element, s));
            if (s == 0) {
                pthread_mutex_lock(&mutex);
                insertToMap(tmp->element, s);
                pthread_mutex_unlock(&mutex);
                pop(q);
            }
        }
    }
    freeStack(&q);
    pthread_mutex_lock(&mutex);
    countEndThreads++;
    pthread_mutex_unlock(&mutex);
}

//Обход дерева в глубину
void depthTreeTraversalWithThreads(nodeptr root)
{
    if (root != NULL) {
        nodeptr leftnode = root->left;
        nodeptr rightnode = root->right;
        cout << root->element;
        if(root->parent != NULL)
           cout << ": parent = "  << root->parent->element;
        cout << " child: ";
        int s = 0;
        if(leftnode != NULL) {
            cout << leftnode->element << " ";
        }
        if(rightnode != NULL) {
            cout << rightnode->element << " ";
        }
        cout << endl;
        depthTreeTraversalWithThreads(leftnode);
        depthTreeTraversalWithThreads(rightnode);
    }
}
int main() {

    int C = 1000;
    pthread_t thread[8];
    int status[8];
    int status_addr[8];
    int threadid[8];
    if (pthread_mutex_init(&mutex,NULL) != 0 )
    {
        printf("Mutex fail!\n");
        return 1;
    }
    InitializeCriticalSection(&CriticalSection);
    summap = new map<int,int>();
    countWorkThreads = 0;
    countEndThreads = 0;

    nodeptr bsroot = NULL;
    nodeptr bsparent = NULL;
    bstree bstree;
    for(int i = 0; i < 100000; i++) {
        bstree.insert(i, bsroot,bsparent);
    }
    cout << "height = " << bstree.bsheight(bsroot) << endl;

    arguments* arg = new arguments;
    arg->id = 0;
    arg->root = bsroot;

    //Время вычисления для всего дерева
    int sum1 = 0;
    long long  int* samples = new long long  int[C];
    for(int k = 0; k < C; k++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        wideTreeTraversalWithThreads(arg);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        sum1+=std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        samples[k]  = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        for (int i = 0; i < 8; i++)
            pthread_map[i].clear();
    }
    cout << endl;
    cout << endl << "Sequence time for all tree: " << sum1/C << endl;
    long long  int dispersia = 0;
    for(int k = 0; k < C; k++) {
        samples[k] = pow((samples[k] - sum1 / C), 2);
        dispersia += samples[k];
    }
    delete[](samples);
    cout << endl << "Sequence dispersia: " << sqrt(dispersia/C) << endl;
    cout << summap->size() << endl;
    cout << endl;
    test4threads(bsroot);
    test8threads(bsroot);
    test2threads(bsroot);
    summap->clear();
    //Время для параллельной обработки 4 потоков
    countEndThreads = 0;
    countWorkThreads = 0;
    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();

    samples = new long long  int[C];
    int sum4 = 0;
    for(int k = 0; k < C; k++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 4; i++) {
            if (i == 0) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->left->left;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 1) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->left->right;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 2) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->right->left;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 3) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->right->right;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            countWorkThreads++;
            pthread_detach(thread[i]);
        }
        while (countEndThreads < countWorkThreads)
            usleep(1);

        int rootsum = bsroot->left->element + bsroot->right->element
                      + bsroot->left->left->element
                      + bsroot->left->right->element
                      + bsroot->right->left->element
                      + bsroot->right->right->element
                      + pthread_map[0].at(bsroot->left->left->element)
                      + pthread_map[1].at(bsroot->left->right->element)
                      + pthread_map[2].at(bsroot->right->left->element)
                      + pthread_map[3].at(bsroot->right->right->element);
        summap->insert(pair<int, int>(bsroot->element, rootsum));
        summap->insert(pair<int, int>(bsroot->left->element,
                                      bsroot->left->left->element + bsroot->left->right->element +
                                      pthread_map[0].at(bsroot->left->left->element)
                                      + pthread_map[1].at(bsroot->left->right->element)));
        summap->insert(pair<int, int>(bsroot->right->element,
                                      bsroot->right->left->element + bsroot->right->right->element +
                                      pthread_map[2].at(bsroot->right->left->element)
                                      + pthread_map[3].at(bsroot->right->right->element)));


        auto end_time = std::chrono::high_resolution_clock::now();

        auto time = end_time - start_time;
        sum4 += std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        samples[k] = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        for(int i = 0; i < 8; i++)
            pthread_map[i].clear();
    }
    cout  <<  endl <<"Parallel time - 4 threads: " << sum4/C << endl;
    dispersia = 0;
    for(int k = 0; k < C; k++) {
        samples[k] = pow((samples[k] - sum4 / C), 2);
        dispersia += samples[k];
    }
    delete[](samples);
    cout << endl << "Parallel dispersia 4 threads: " << sqrt(dispersia/C) << endl;
    cout << summap->size() << endl;
    //Время параллельного выисления для 8 потоков
    countEndThreads = 0;
    countWorkThreads = 0;

    summap->clear();
    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();

    int sum8 = 0;
    samples = new long long  int[C];
    for(int k = 0; k < C; k++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 8; i++) {
            if (i == 0) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->left->left->left;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 1) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->left->left->right;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 2) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->left->right->left;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 3) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->left->right->right;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 4) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->right->left->left;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 5) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->right->left->right;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 6) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->right->right->left;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 7) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->right->right->right;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            countWorkThreads++;
            pthread_detach(thread[i]);
        }
        while (countEndThreads < countWorkThreads)
            usleep(1);

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
                  + pthread_map[0].at(bsroot->left->left->left->element)
                  + pthread_map[1].at(bsroot->left->left->right->element)
                  + pthread_map[2].at(bsroot->left->right->left->element)
                  + pthread_map[3].at(bsroot->left->right->right->element)
                  + pthread_map[4].at(bsroot->right->left->left->element)
                  + pthread_map[5].at(bsroot->right->left->right->element)
                  + pthread_map[6].at(bsroot->right->right->left->element)
                  + pthread_map[7].at(bsroot->right->right->right->element);
        summap->insert(pair<int, int>(bsroot->element, rootsum));
        summap->insert(pair<int, int>(bsroot->left->element, bsroot->left->left->element
                                                             + bsroot->left->right->element
                                                             + bsroot->left->left->left->element
                                                             + bsroot->left->left->right->element
                                                             + pthread_map[0].at(bsroot->left->left->left->element)
                                                             + pthread_map[1].at(bsroot->left->left->right->element)
                                                             + bsroot->left->right->left->element
                                                             + bsroot->left->right->right->element
                                                             + pthread_map[2].at(bsroot->left->right->left->element)
                                                             + pthread_map[3].at(bsroot->left->right->right->element)));
        summap->insert(pair<int, int>(bsroot->right->element, bsroot->right->left->element
                                                              + bsroot->right->right->element
                                                              + bsroot->right->left->left->element
                                                              + bsroot->right->left->right->element
                                                              + pthread_map[4].at(bsroot->right->left->left->element)
                                                              + pthread_map[5].at(bsroot->right->left->right->element)
                                                              + bsroot->right->right->left->element
                                                              + bsroot->right->right->right->element
                                                              + pthread_map[6].at(bsroot->right->right->left->element)
                                                              +
                                                              pthread_map[7].at(bsroot->right->right->right->element)));
        summap->insert(pair<int, int>(bsroot->left->left->element,
                                      bsroot->left->left->left->element
                                      + bsroot->left->left->right->element
                                      + pthread_map[0].at(bsroot->left->left->left->element)
                                      + pthread_map[1].at(bsroot->left->left->right->element)));
        summap->insert(pair<int, int>(bsroot->left->right->element,
                                      bsroot->left->right->left->element
                                      + bsroot->left->right->right->element
                                      + pthread_map[2].at(bsroot->left->right->left->element)
                                      + pthread_map[3].at(bsroot->left->right->right->element)));
        summap->insert(pair<int, int>(bsroot->right->left->element,
                                      bsroot->right->left->left->element
                                      + bsroot->right->left->right->element
                                      + pthread_map[4].at(bsroot->right->left->left->element)
                                      + pthread_map[5].at(bsroot->right->left->right->element)));
        summap->insert(pair<int, int>(bsroot->right->right->element,
                                      bsroot->right->right->left->element
                                      + bsroot->right->right->right->element
                                      + pthread_map[6].at(bsroot->right->right->left->element)
                                      + pthread_map[7].at(bsroot->right->right->right->element)));

        auto end_time = std::chrono::high_resolution_clock::now();

        auto time = end_time - start_time;
        sum8 += std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        samples[k] = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        for(int i = 0; i < 8; i++)
            pthread_map[i].clear();
    }
    cout  <<  endl <<"Parallel time - 8 threads: " << sum8/C << endl;
    dispersia = 0;
    for(int k = 0; k < C; k++) {
        samples[k] = pow((samples[k] - sum8 / C), 2);
        dispersia += samples[k];
    }
    delete[](samples);
    cout << endl << "Parallel dispersia 8 threads: " << sqrt(dispersia/C) << endl;

    cout << summap->size() << endl;

    //Время параллельного выисления для 2 потоков
    countEndThreads = 0;
    countWorkThreads = 0;
    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();

    summap->clear();
    int sum2 = 0;
    samples = new long long  int[C];
    for(int k = 0; k < C; k++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 2; i++) {
            if (i == 0) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->left;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            else if (i == 1) {
                arguments *arg = new arguments;
                arg->id = i;
                arg->root = bsroot->right;
                if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void *) arg)) != 0) {
                    printf("Can't create thread!\n");
                }
            }
            countWorkThreads++;
            pthread_detach(thread[i]);
        }
        while (countEndThreads < countWorkThreads)
            usleep(1);
        int rootsum = bsroot->left->element + bsroot->right->element + pthread_map[0].at(bsroot->left->element)
                  + pthread_map[1].at(bsroot->right->element);
        summap->insert(pair<int, int>(bsroot->element, rootsum));
        auto end_time = std::chrono::high_resolution_clock::now();

        auto time = end_time - start_time;
        sum2 += std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        samples[k] = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        for(int i = 0; i < 8; i++)
            pthread_map[i].clear();
    }
    cout  <<  endl <<"Parallel time - 2 threads: " << sum2/C << endl;
    dispersia = 0;
    for(int k = 0; k < C; k++) {
        samples[k] = pow((samples[k] - sum2 / C), 2);
        dispersia += samples[k];
    }
    delete[](samples);
    cout << endl << "Parallel dispersia 2 threads: " << sqrt(dispersia/C) << endl;

    cout << summap->size() << endl;
    //Освобождаем ресурсы
    pthread_mutex_destroy(&mutex);
    delete(summap);


    return 0;
}