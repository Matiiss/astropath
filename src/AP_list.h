/* NOT a linked list */

#pragma once

#include <stdlib.h>

#define AP_LIST_CHUNK 32

typedef struct AP_List AP_List;

struct AP_List {
    void **array;
    size_t length;  // item count in the array
    size_t _size;  // allocated size

    int (*append)(AP_List *self, void *item);
    void *(*get_at)(AP_List *self, size_t index);
    int (*set_at)(AP_List *self, size_t index, void *item);
    void *(*pop)(AP_List *self, size_t index);
    void *(*pop_end)(AP_List *self);
    void (*free)(AP_List *self);
};

int AP_ListInit(AP_List *self);
int AP_ListAppend(AP_List *self, void *item);
void *AP_ListGetAt(AP_List *self, size_t index);
int AP_ListSetAt(AP_List *self, size_t index, void *item);
void *AP_ListPop(AP_List *self, size_t index);
void *AP_ListPopEnd(AP_List *self);
void AP_ListFree(AP_List *self);
