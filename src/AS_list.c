#include <stdio.h>
#include <stdlib.h>

#include "./AS_list.h"

int AS_ListInit(AS_List *self) {
    self->array = (void **)malloc(AS_LIST_CHUNK * sizeof(void *));

    if (!self->array) {
        return 1;
    }

    self->length = 0;
    self->_size = AS_LIST_CHUNK;

    self->append = &AS_ListAppend;
    self->get_at = &AS_ListGetAt;
    self->set_at = &AS_ListSetAt;
    self->pop = &AS_ListPop;
    self->pop_end = &AS_ListPopEnd;
    self->free = &AS_ListFree;

    return 0;
}

int AS_ListAppend(AS_List *self, void *item) {
    self->length++;
    if (self->length > self->_size) {
        self->_size += AS_LIST_CHUNK;
        self->array = realloc(self->array, self->_size * sizeof(void *));
        if (!self->array) {
            return 1;
        }
    }

    self->array[self->length - 1] = item;
    return 0;
}

void *AS_ListGetAt(AS_List *self, size_t index) {
    if (index >= self->length) {
        return NULL;
    }

    return self->array[index];
}

int AS_ListSetAt(AS_List *self, size_t index, void *item) {
    if (index >= self->length) {
        return 1;
    }

    self->array[index] = item;
    return 0;
}

void *AS_ListPop(AS_List *self, size_t index) {
    void *item = self->get_at(self, index);
    if (!item) {
        return NULL;
    }

    self->length--;
    for (size_t i = index; i < self->length; ++i) {
        self->array[i] = self->array[i + 1];
    }

    return item;
}

void *AS_ListPopEnd(AS_List *self) {
    return self->pop(self, self->length - 1);
}

void AS_ListFree(AS_List *self) {
    free(self->array);
}
