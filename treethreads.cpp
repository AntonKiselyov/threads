//
// Created by Admin on 01.03.2016.
//

#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <unistd.h>
#include <map>
#include "avl.h"
#include "stack.h"
using namespace std;

pthread_mutex_t mutex;
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



void insertToMap(int &n, int &s)
{
    summap->insert(pair<int,int>(n,s));
}
//Обход дерева в ширину
void* wideTreeTraversalWithThreads(void* arg) {
    int count = 0;
   // auto start_time = std::chrono::high_resolution_clock::now();

    Stack *q = createStack();
    map<int,int> threadmap;
    push(q,( (arguments*)arg )->root);
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
        //pthread_mutex_lock(&mutex);
        //summap->insert(pair<int,int>(tmp->element, s));
        pthread_map[( (arguments*)arg )->id].insert(pair<int,int>(tmp->element, s));
        //threadmap.insert(pair<int,int>(tmp->element, s));
        while(tmp != ( (arguments*)arg )->root)
        {
            tmp = tmp->parent;
            /* int v = summap->at(tmp->element);
             summap->erase(tmp->element);
             summap->insert(pair<int,int>(tmp->element,v + s));*/
            int v = pthread_map[( (arguments*)arg )->id].at(tmp->element);
            pthread_map[( (arguments*)arg )->id].erase(tmp->element);
            pthread_map[( (arguments*)arg )->id].insert(pair<int,int>(tmp->element,v + s));
            /*int v = threadmap.at(tmp->element);
            threadmap.erase(tmp->element);
            threadmap.insert(pair<int,int>(tmp->element,v + s));*/
        }
       // pthread_mutex_unlock(&mutex);
        count++;
    }
    freeStack(&q);

   /* auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time-start_time;
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long mslong = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000; //get current timestamp in milliseconds*/
    //cout  <<  endl<<"Thread "<< ((nodeptr)arg)->element <<" have time: "<<  " " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " count: " << count << " end time: " << mslong << std::endl<< endl;
    pthread_mutex_lock(&mutex);
    countEndThreads++;
    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);
}

int main() {
    pthread_t thread[8];
    int status[8];
    int status_addr[8];
    int threadid[8];
    if (pthread_mutex_init(&mutex,NULL) != 0 )
    {
        printf("Mutex fail!\n");
        return 1;
    }
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


    //Время вычисления для всего дерева
    auto start_time = std::chrono::high_resolution_clock::now();
    arguments* arg = new arguments;
    arg->id = 0;
    arg->root = bsroot;
    wideTreeTraversalWithThreads(arg);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time-start_time;
    cout << endl;
    cout << endl << "Sequence time for all tree: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;
    for(int i = 0; i < pthread_map[0].size(); i++)
        summap->insert(pair<int,int>(i,pthread_map[0].at(i)));
    cout << summap->size() << endl;

    test4threads(bsroot);
    test8threads(bsroot);
    test2threads(bsroot);

    //Время для параллельной обработки 4 потоков
    countEndThreads = 0;
    countWorkThreads = 0;
    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();

    start_time = std::chrono::high_resolution_clock::now();
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

    int rootsum = bsroot->left->element + bsroot->right->element
                  + pthread_map[0].at(bsroot->left->left->element)
                    + pthread_map[1].at(bsroot->left->right->element)
                      + pthread_map[2].at(bsroot->right->left->element)
                        + pthread_map[3].at(bsroot->right->right->element);
    summap->insert(pair<int,int> (bsroot->element,rootsum));
    summap->insert(pair<int,int> (bsroot->left->element,pthread_map[0].at(bsroot->left->left->element)
                                                        + pthread_map[1].at(bsroot->left->right->element)));
    summap->insert(pair<int,int> (bsroot->right->element,pthread_map[2].at(bsroot->right->left->element)
                                                         + pthread_map[3].at(bsroot->right->right->element)));
    end_time = std::chrono::high_resolution_clock::now();

    time = end_time-start_time;

    cout  <<  endl <<"Parallel time - 4 threads: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

    //Время параллельного выисления для 8 потоков
    countEndThreads = 0;
    countWorkThreads = 0;

    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();
    start_time = std::chrono::high_resolution_clock::now();
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

    rootsum = bsroot->left->element + bsroot->right->element
                  + bsroot->left->left->element + bsroot->left->right->element
                  + bsroot->right->left->element + bsroot->right->right->element
                  + pthread_map[0].at(bsroot->left->left->left->element)
                    + pthread_map[1].at(bsroot->left->left->right->element)
                      + pthread_map[2].at(bsroot->left->right->left->element)
                        + pthread_map[3].at(bsroot->left->right->right->element)
                          + pthread_map[4].at(bsroot->right->left->left->element)
                            + pthread_map[5].at(bsroot->right->left->right->element)
                              + pthread_map[6].at(bsroot->right->right->left->element)
                                + pthread_map[7].at(bsroot->right->right->right->element);
    summap->insert(pair<int,int> (bsroot->element,rootsum));

    end_time = std::chrono::high_resolution_clock::now();

    time = end_time-start_time;

    cout  <<  endl <<"Parallel time - 8 threads: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

    //Время параллельного выисления для 2 потоков
    countEndThreads = 0;
    countWorkThreads = 0;
    for(int i = 0; i < 8; i++)
        pthread_map[i].clear();

    start_time = std::chrono::high_resolution_clock::now();
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

    rootsum = pthread_map[0].at(bsroot->left->element)
                      + pthread_map[1].at(bsroot->right->element);
    summap->insert(pair<int,int> (bsroot->element,rootsum));
    end_time = std::chrono::high_resolution_clock::now();

    time = end_time-start_time;

    cout  <<  endl <<"Parallel time - 2 threads: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

    //Освобождаем ресурсы
    pthread_mutex_destroy(&mutex);
    delete(summap);


    return 0;
}