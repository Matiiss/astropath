typedef struct AS_Stack AS_Stack;
typedef struct AS_StackNode AS_StackNode;

struct AS_Stack {
    AS_StackNode *head;

    void (*push)(AS_Stack *self, void *data);
    void *(*pop)(AS_Stack *self);
    void (*free)(AS_Stack *self);
};

struct AS_StackNode {
    void *data;
    AS_StackNode *next;
};

void AS_StackInit(AS_Stack *self);
void AS_StackPush(AS_Stack *self, void *data);
void *AS_StackPop(AS_Stack *self);
void AS_StackFree(AS_Stack *self);
