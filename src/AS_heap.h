#pragma once

#include <stdlib.h>

typedef struct AS_Heap AS_Heap;

struct AS_Heap {
    void **tree;
    size_t length;
    size_t size;
    int (*compare)(void *primary, void *secondary);

    int (*push)(AS_Heap *self, void *element);
    void *(*pop)(AS_Heap *self);
    void (*heapify)(AS_Heap *self);
    void (*free)(AS_Heap *self);
};

int AS_HeapInit(AS_Heap *self, int (compare)(void *primary, void *secondary));
int AS_HeapPush(AS_Heap *self, void *element);
void *AS_HeapPop(AS_Heap *self);
void AS_HeapHeapify(AS_Heap *self);
void AS_HeapFree(AS_Heap *self);
