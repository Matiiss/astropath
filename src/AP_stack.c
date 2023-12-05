#include <stdio.h>
#include <stdlib.h>

#include "./AP_stack.h"

void AP_StackInit(AP_Stack *self) {
    self->head = NULL;
    self->size = 0;

    self->push = &AP_StackPush;
    self->pop = &AP_StackPop;
    self->free = &AP_StackFree;
}

int AP_StackPush(AP_Stack *self, void *data) {
    AP_StackNode *new_node = malloc(sizeof(AP_StackNode));

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

void *AP_StackPop(AP_Stack *self) {
    if (self->head == NULL) {
        return NULL;
    }

    AP_StackNode *node = self->head;
    void *data = node->data;
    self->head = node->next;
    free(node);

    self->size--;

    return data;
}

void AP_StackFree(AP_Stack *self) {
    AP_StackNode *node = self->head, *next;
    self->head = NULL;

    while (node) {
        next = node->next;
        free(node);
        node = next;
    }
}