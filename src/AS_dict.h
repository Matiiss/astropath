#pragma once

#include <stdlib.h>

#include "./AS_list.h"

typedef struct AS_Dict AS_Dict;
typedef struct AS_DictNode AS_DictNode;
typedef long long AS_Hash;
typedef long long (*AS_HashFunc)(void *key);
typedef int (*AS_DictEqCheck)(void *key1, void *key2);

struct AS_DictNode {
    int state;
    AS_Hash hash;
    void *key;
    void *item;
};

struct AS_Dict {
    AS_List *nodes;
    size_t using;
    size_t fullness;
    AS_HashFunc hash;
    AS_DictEqCheck eq_check;

    void *(*get)(AS_Dict *self, void *key);
    int (*set)(AS_Dict *self, void *key, void *item);
    void (*del)(AS_Dict *self, void *key);
    void (*free)(AS_Dict *self);
};

int AS_DictInit(AS_Dict *self, AS_HashFunc hash, AS_DictEqCheck eq_check);
void *AS_DictGet(AS_Dict *self, void *key);
int AS_DictSet(AS_Dict *self, void *key, void *item);
int AS_DictDel(AS_Dict *self, void *key);
void AS_DictFree(AS_Dict *self);
