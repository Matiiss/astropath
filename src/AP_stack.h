#pragma once

typedef struct AP_Stack AP_Stack;
typedef struct AP_StackNode AP_StackNode;

struct AP_Stack {
    AP_StackNode *head;
    size_t size;

    int (*push)(AP_Stack *self, void *data);
    void *(*pop)(AP_Stack *self);
    void (*free)(AP_Stack *self);
};

struct AP_StackNode {
    void *data;
    AP_StackNode *next;
};

void AP_StackInit(AP_Stack *self);
int AP_StackPush(AP_Stack *self, void *data);
void *AP_StackPop(AP_Stack *self);
void AP_StackFree(AP_Stack *self);
