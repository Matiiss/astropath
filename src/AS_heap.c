#include <stdlib.h>
#include <stdio.h>

#include "./AS_heap.h"

// int CompareNumbersLT(void *primary, void *secondary);
// int CompareNumbersGT(void *primary, void *secondary);

// int main() {
//     int arr[8] = {10, 20, 25, 6, 12, 15, 4, 16};

//     AS_Heap heap_obj;
//     AS_Heap *heap = &heap_obj;
//     AS_HeapInit(heap, &CompareNumbersLT);

//     for (int i = 0; i < 8; ++i) {
//         heap->push(heap, (void *)(arr + i));
//     }

//     for (int i = 0; i < 8; ++i) {
//         printf("%i ", *(int *)heap->pop(heap));
//     }

//     return 0;
// }

// int CompareNumbersLT(void *primary, void *secondary) {
//     return *(int *)primary < *(int *)secondary;
// }

// int CompareNumbersGT(void *primary, void *secondary) {
//     return *(int *)primary > *(int *)secondary;
// }

int AS_HeapInit(AS_Heap *self, int (*compare)(void *primary, void *secondary)) {
    self->tree = (void **)malloc(sizeof(void *) * AS_HEAP_CHUNK);

    if (!self->tree) {
        return 1;
    }

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

    return 0;
}

void *AS_HeapPop(AS_Heap *self) {
    void *top = self->tree[0];
    self->tree[0] = self->tree[self->length - 1];
    self->tree[self->length - 1] = NULL;
    self->length--;

    return top;
}

void AS_HeapHeapify(AS_Heap *self) {
    for (size_t index = self->length / 2; index >= 1; --index) {
        _AS_HeapHeapify(self, index - 1);
    }
}

void _AS_HeapHeapify(AS_Heap *self, size_t index) {
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

        _AS_HeapHeapify(self, next);
    }
}

void AS_HeapFree(AS_Heap *self) {
    free(self->tree);
}
