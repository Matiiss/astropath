/* NOT a linked list */

#include <stdlib.h>

#define AS_LIST_CHUNK 32

typedef struct AS_List AS_List;

struct AS_List {
    void **array;
    size_t length;  // item count in the array
    size_t _size;  // allocated size

    int (*append)(AS_List *self, void *item);
    void *(*get_at)(AS_List *self, size_t index);
    int (*set_at)(AS_List *self, size_t index, void *item);
    void *(*pop)(AS_List *self, size_t index);
    void *(*pop_end)(AS_List *self);
    void (*free)(AS_List *self);
};

int AS_ListInit(AS_List *self);
int AS_ListAppend(AS_List *self, void *item);
void *AS_ListGetAt(AS_List *self, size_t index);
int AS_ListSetAt(AS_List *self, size_t index, void *item);
void *AS_ListPop(AS_List *self, size_t index);
void *AS_ListPopEnd(AS_List *self);
void AS_ListFree(AS_List *self);
