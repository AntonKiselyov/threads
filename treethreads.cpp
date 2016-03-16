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
map<int,int>* pthread_map = new map<int,int>[4];

struct arguments {
    int id;
    nodeptr root;
};

int countWorkThreads;//Число работающих потоков
int countEndThreads;//Число законченных потоков

void* wideTreeTraversalWithThreads(void* arg);

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
        //pthread_map[( (arguments*)arg )->id].insert(pair<int,int>(tmp->element, s));
        threadmap.insert(pair<int,int>(tmp->element, s));
        while(tmp != ( (arguments*)arg )->root)
        {
            tmp = tmp->parent;
            /* int v = summap->at(tmp->element);
             summap->erase(tmp->element);
             summap->insert(pair<int,int>(tmp->element,v + s));*/
            /*int v = pthread_map[( (arguments*)arg )->id].at(tmp->element);
            pthread_map[( (arguments*)arg )->id].erase(tmp->element);
            pthread_map[( (arguments*)arg )->id].insert(pair<int,int>(tmp->element,v + s));*/
            int v = threadmap.at(tmp->element);
            threadmap.erase(tmp->element);
            threadmap.insert(pair<int,int>(tmp->element,v + s));
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

    auto start_time = std::chrono::high_resolution_clock::now();
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

  /*  int rootsum = bsroot->left->element + bsroot->right->element
                  + pthread_map[0].at(bsroot->left->left->element)
                    + pthread_map[1].at(bsroot->left->right->element)
                      + pthread_map[2].at(bsroot->right->left->element)
                        + pthread_map[3].at(bsroot->right->right->element);
    summap->insert(pair<int,int> (bsroot->element,rootsum));
    cout << rootsum;*/
    pthread_mutex_destroy(&mutex);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto time = end_time-start_time;

    cout  <<  endl<<"Parallel time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

    auto full_time_start = std::chrono::high_resolution_clock::now();

    //start_time = std::chrono::high_resolution_clock::now();
    arguments* arg = new arguments;
    arg->id = 0;
    arg->root = bsroot->left->left;
    wideTreeTraversalWithThreads(arg);
    //end_time = std::chrono::high_resolution_clock::now();

    //time = end_time-start_time;

    //cout <<"time: " <<std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

  //  cout << endl;
   // start_time = std::chrono::high_resolution_clock::now();
    arg = new arguments;
    arg->id = 1;
    arg->root = bsroot->left->right;
    wideTreeTraversalWithThreads(arg);
   // end_time = std::chrono::high_resolution_clock::now();

   // time = end_time-start_time;

   // cout <<"time: "<< std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;


   // cout << endl;

    //start_time = std::chrono::high_resolution_clock::now();
    arg = new arguments;
    arg->id = 2;
    arg->root = bsroot->right->left;
    wideTreeTraversalWithThreads(arg);
    //end_time = std::chrono::high_resolution_clock::now();

    //time = end_time-start_time;

    //cout <<"time: "<< std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

    //cout << endl;

    //start_time = std::chrono::high_resolution_clock::now();
    arg = new arguments;
    arg->id = 3;
    arg->root = bsroot->right->right;
    wideTreeTraversalWithThreads(arg);
    //end_time = std::chrono::high_resolution_clock::now();

   // time = end_time-start_time;

   // cout <<"time: "<< std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

   // cout << endl;

    auto full_time_end = std::chrono::high_resolution_clock::now();

    auto full_time = full_time_end- full_time_start;

    cout << "full sequence time: " << std::chrono::duration_cast<std::chrono::microseconds>(full_time).count() << endl;


    start_time = std::chrono::high_resolution_clock::now();
    arg = new arguments;
    arg->id = 0;
    arg->root = bsroot;
    wideTreeTraversalWithThreads(arg);
    end_time = std::chrono::high_resolution_clock::now();
    time = end_time-start_time;
    cout << endl;
    cout << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;
    delete(summap);


    return 0;
}