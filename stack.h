//
// Created by Admin on 24.02.2016.
//

#ifndef THREADS_STACK_H
#define THREADS_STACK_H
#include "avl.h"
#include <map>

#define STACK_INIT_SIZE 25000000

void depthTreeTraversal(nodeptr, map<int,int>&);
void wideTreeTraversal(nodeptr, map<int,int>&);
void insertToMap(int &, int &, map<int,int> &);

typedef struct Stack {
    size_t size;
    size_t limit;
    nodeptr *data;
} Stack;

Stack* createStack() {
    Stack *tmp = (Stack*) malloc(sizeof(Stack));
    tmp->limit = STACK_INIT_SIZE;
    tmp->size = 0;
    tmp->data = (nodeptr*) malloc(tmp->limit * sizeof(nodeptr));
    return tmp;
}

void freeStack(Stack **s) {
    free((*s)->data);
    free(*s);
    *s = NULL;
}

void push(Stack *s, nodeptr item) {
    if (s->size >= s->limit) {
        s->limit *= 2;
        s->data = (nodeptr*) realloc(s->data, s->limit * sizeof(nodeptr));
    }
    s->data[s->size++] = item;
}

nodeptr pop(Stack *s) {
    if (s->size == 0) {
        return NULL;
    }
    else {
        s->size--;
        return s->data[s->size];
    }
}

nodeptr peek(Stack *s) {
    return s->data[s->size-1];
}

#endif //THREADS_STACK_H
