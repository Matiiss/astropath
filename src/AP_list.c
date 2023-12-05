#include <stdio.h>
#include <stdlib.h>

#include "./AP_list.h"

int AP_ListInit(AP_List *self) {
    self->array = (void **)malloc(AP_LIST_CHUNK * sizeof(void *));

    if (!self->array) {
        return 1;
    }

    self->length = 0;
    self->_size = AP_LIST_CHUNK;

    self->append = &AP_ListAppend;
    self->get_at = &AP_ListGetAt;
    self->set_at = &AP_ListSetAt;
    self->pop = &AP_ListPop;
    self->pop_end = &AP_ListPopEnd;
    self->free = &AP_ListFree;

    return 0;
}

int AP_ListAppend(AP_List *self, void *item) {
    self->length++;
    if (self->length > self->_size) {
        self->_size += AP_LIST_CHUNK;
        self->array = realloc(self->array, self->_size * sizeof(void *));
        if (!self->array) {
            return 1;
        }
    }

    self->array[self->length - 1] = item;
    return 0;
}

void *AP_ListGetAt(AP_List *self, size_t index) {
    if (index >= self->length) {
        return NULL;
    }

    return self->array[index];
}

int AP_ListSetAt(AP_List *self, size_t index, void *item) {
    if (index >= self->length) {
        return 1;
    }

    self->array[index] = item;
    return 0;
}

void *AP_ListPop(AP_List *self, size_t index) {
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

void *AP_ListPopEnd(AP_List *self) {
    return self->pop(self, self->length - 1);
}

void AP_ListFree(AP_List *self) {
    free(self->array);
}
