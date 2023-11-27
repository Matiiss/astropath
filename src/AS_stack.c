#include <stdio.h>
#include <stdlib.h>

#include "./AS_stack.h"

// int main() {
//     AS_Stack stack_obj, *stack;
//     stack = &stack_obj;
//     AS_StackInit(stack);

//     int arr[5] = {1, 2, 7, 4, 5};

//     for (int i = 0; i < 5; ++i) {
//         stack->push(stack, (void *)&arr[i]);
//     }

//     void *data;
//     while ((data = stack->pop(stack))) {
//         printf("%i ", *(int *)data);
//     }

//     stack->free(stack);

//     return 0;
// }

void AS_StackInit(AS_Stack *self) {
    self->head = NULL;

    self->push = &AS_StackPush;
    self->pop = &AS_StackPop;
    self->free = &AS_StackFree;
}

void AS_StackPush(AS_Stack *self, void *data) {
    AS_StackNode *new_node = malloc(sizeof(AS_StackNode));
    new_node->data = data;
    
    if (self->head == NULL) {
        new_node->next = NULL;
    } else {
        new_node->next = self->head;
    }

    self->head = new_node;
}

void *AS_StackPop(AS_Stack *self) {
    if (self->head == NULL) {
        return NULL;
    }

    AS_StackNode *node = self->head;
    void *data = node->data;
    self->head = node->next;
    free(node);

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