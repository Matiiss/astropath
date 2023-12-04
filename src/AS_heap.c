#include <stdlib.h>
#include <stdio.h>

#include "./AS_heap.h"

#define AS_HEAP_CHUNK 32

/*
reference implementation for siftdown and siftup:
https://github.com/python/cpython/blob/main/Lib/heapq.py#L207
https://github.com/python/cpython/blob/main/Lib/heapq.py#L260
*/

void siftdown(AS_Heap *self, size_t end_pos, size_t pos);
void siftup(AS_Heap *self, size_t pos);
void heapify(AS_Heap *self, size_t index);

int AS_HeapInit(AS_Heap *self, int (*compare)(void *primary, void *secondary)) {
    self->tree = (void **)malloc(sizeof(void *) * AS_HEAP_CHUNK);

    if (!self->tree) {
        return 1;
    }
    self->tree[0] = NULL;

    self->length = 0;
    self->size = AS_HEAP_CHUNK;
    self->compare = compare;

    self->push = &AS_HeapPush;
    self->pop = &AS_HeapPop;
    self->heapify = &AS_HeapHeapify;
    self->free = &AS_HeapFree;

    return 0;
}

int AS_HeapPush(AS_Heap *self, void *element) {
    self->length++;
    if (self->length > self->size) {
        self->size += AS_HEAP_CHUNK;
        self->tree = (void **)realloc(self->tree, sizeof(void *) * self->size);

        if (!self->tree) {
            return 1;
        }
    }

    self->tree[self->length - 1] = element;
    siftdown(self, 0, self->length - 1);

    return 0;
}

void *AS_HeapPop(AS_Heap *self) {
    void *top = self->tree[0];
    if (!top) {
        return NULL;
    }

    self->tree[0] = self->tree[self->length - 1];
    self->tree[self->length - 1] = NULL;
    self->length--;

    siftup(self, 0);

    return top;
}

void AS_HeapHeapify(AS_Heap *self) {
    for (size_t index = self->length / 2; index >= 1; --index) {
        heapify(self, index - 1);
    }
}

void siftdown(AS_Heap *self, size_t end_pos, size_t pos) {
    void *current = self->tree[pos];

    while (pos > end_pos) {
        size_t parent_pos = (pos - 1) >> 1;
        if (self->compare(current, self->tree[parent_pos])) {
            self->tree[pos] = self->tree[parent_pos];
            pos = parent_pos;
        } else {
            break;
        }
    }

    self->tree[pos] = current;
}

void siftup(AS_Heap *self, size_t pos) {
    size_t end_pos = self->length;
    size_t start_pos = pos;
    void *current = self->tree[pos];

    size_t child = pos * 2 + 1;  // left node
    while (child < end_pos) {
        size_t right_node = child + 1;

        if (right_node < end_pos) {
            if (self->compare(self->tree[right_node], self->tree[child])) {
                child = right_node;
            }
        }

        self->tree[pos] = self->tree[child];
        pos = child;
        child = 2 * pos + 1;
    }

    self->tree[pos] = current;
    siftdown(self, start_pos, pos);
}

void heapify(AS_Heap *self, size_t index) {
    size_t left = index * 2 + 1;
    size_t right = index * 2 + 2;
    size_t next = index;

    if (left < self->length) {
        if (self->compare(self->tree[left], self->tree[index])) {
            next = left;
        }
    }

    if (right < self->length) {
        if (self->compare(self->tree[right], self->tree[next])) {
            next = right;
        }
    }

    if (next != index) {
        void *tmp = self->tree[next];
        self->tree[next] = self->tree[index];
        self->tree[index] = tmp;

        heapify(self, next);
    }
}

void AS_HeapFree(AS_Heap *self) {
    free(self->tree);
}
