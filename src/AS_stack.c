#include <stdio.h>
#include <stdlib.h>

#include "./AS_stack.h"

void AS_StackInit(AS_Stack *self) {
    self->head = NULL;
    self->size = 0;

    self->push = &AS_StackPush;
    self->pop = &AS_StackPop;
    self->free = &AS_StackFree;
}

int AS_StackPush(AS_Stack *self, void *data) {
    AS_StackNode *new_node = malloc(sizeof(AS_StackNode));

    if (!new_node) {
        return 1;
    }

    new_node->data = data;
    self->size++;
    
    if (self->head == NULL) {
        new_node->next = NULL;
    } else {
        new_node->next = self->head;
    }

    self->head = new_node;
    return 0;
}

void *AS_StackPop(AS_Stack *self) {
    if (self->head == NULL) {
        return NULL;
    }

    AS_StackNode *node = self->head;
    void *data = node->data;
    self->head = node->next;
    free(node);

    self->size--;

    return data;
}

void AS_StackFree(AS_Stack *self) {
    AS_StackNode *node = self->head, *next;
    self->head = NULL;

    while (node) {
        next = node->next;
        free(node);
        node = next;
    }
}