#include <stdio.h>
#include <stdlib.h>

#include "./AS_list.h"

// int main() {
//     int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

//     AS_List list_obj;
//     AS_List *list = &list_obj;
//     AS_ListInit(list);

//     for (int i = 0; i < 10; ++i) {
//         if (list->append(list, (void *)&arr[i])) {
//             return 1;
//         }
//     }

//     printf("length: %lli\n", list->length);
//     printf("%i, %i, %i\n", *(int *)list->get_at(list, 2), *(int *)list->get_at(list, 5), *(int *)list->get_at(list, 7));
//     printf("%i\n", *(int *)list->pop(list, 3));
//     printf("length: %lli\n", list->length);
//     printf("%i\n", *(int *)list->pop_end(list));
//     printf("length: %lli\n", list->length);

//     list->free(list);

//     return 0;
// }

int AS_ListInit(AS_List *self) {
    self->array = (void **)malloc(AS_LIST_CHUNK * sizeof(void *));

    if (!self->array) {
        return 1;
    }

    self->length = 0;
    self->_size = AS_LIST_CHUNK;

    self->append = &AS_ListAppend;
    self->get_at = &AS_ListGetAt;
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
