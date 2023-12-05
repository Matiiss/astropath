#pragma once

#include <stdlib.h>

typedef struct AP_Heap AP_Heap;

struct AP_Heap {
    void **tree;
    size_t length;
    size_t size;
    int (*compare)(void *primary, void *secondary);

    int (*push)(AP_Heap *self, void *element);
    void *(*pop)(AP_Heap *self);
    void (*heapify)(AP_Heap *self);
    void (*free)(AP_Heap *self);
};

int AP_HeapInit(AP_Heap *self, int (compare)(void *primary, void *secondary));
int AP_HeapPush(AP_Heap *self, void *element);
void *AP_HeapPop(AP_Heap *self);
void AP_HeapHeapify(AP_Heap *self);
void AP_HeapFree(AP_Heap *self);
