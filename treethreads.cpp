//
// Created by Admin on 01.03.2016.
//

#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include "stack.h"
using namespace std;

pthread_mutex_t mutex;
map<int,int>* summap;
int* arr = new int[1000000];
Stack *q0;
Stack *q1;
Stack *q2;
Stack *q3;

int countWorkThreads;
int countEndThreads;

void* wideTreeTraversalWithThreads(void* arg);

void insertToMap(int &n, int &s)
{
    summap->insert(pair<int,int>(n,s));
}
//Обход дерева в ширину
void* wideTreeTraversalWithThreads(void* arg) {
    Stack *q = createStack();
    push(q, ((Node*) arg));
    while (q->size != 0) {
        Node *tmp = pop(q);
        int s = 0;
        if (tmp->left != NIL) {
            push(q, tmp->left);
            s += tmp->left->data;
        }
        if (tmp->right != NIL) {
            push(q, tmp->right);
            s += tmp->right->data;
        }
        //pthread_mutex_lock(&mutex);
       // insertToMap(tmp->data,s);
        //arr[tmp->data] = s;
        ///pthread_mutex_unlock(&mutex);
    }
    freeStack(&q);
    pthread_mutex_lock(&mutex);
    countEndThreads++;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


int main() {
    q0 = createStack();
    q1 = createStack();
    q2 = createStack();
    q3 = createStack();
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

    for (int i = 100000; i >= 0; i--)
        insertNode(i);
    for(int i = 0; i < 8; i++)
        threadid[i] = i;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 4; i++)
    {
        if(i == 0) {
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void*)root->left->left)) != 0)
            {
                printf("Can't create thread!\n");
            }
        }
        else if(i == 1) {
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void*)root->left->right)) != 0)
            {
                printf("Can't create thread!\n");
            }
        }
        else if(i == 2) {
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void*)root->right->left)) != 0)
            {
                printf("Can't create thread!\n");
            }
        }
        else if(i == 3) {
            if ((status[i] = pthread_create(&thread[i], NULL, &wideTreeTraversalWithThreads, (void*)root->right->right)) != 0)
            {
                printf("Can't create thread!\n");
            }
        }
        countWorkThreads++;
        pthread_detach(thread[i]);
    }
    while(countEndThreads < countWorkThreads)
        usleep(10);
   /* for(int i = 0; i < 4; i++)
    {
        if ((status_addr[i] = pthread_join(thread[i], NULL)) != 0)
        {
            printf("Can't join thread!\n");
        }
    }*/

    auto end_time = std::chrono::high_resolution_clock::now();

    auto time = end_time-start_time;

    cout << endl << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << endl;

    printf("%d\n",summap->size());
    /*for(int i = 0; i< 1000000; i++)
        cout << arr[i] << " ";*/
    pthread_mutex_destroy(&mutex);
    delete(summap);
    return 0;
}