#pragma once

#include <stdlib.h>

#include "./AP_list.h"

typedef struct AP_Dict AP_Dict;
typedef struct AP_DictNode AP_DictNode;
typedef long long AP_Hash;
typedef long long (*AP_HashFunc)(void *key);
typedef int (*AP_DictEqCheck)(void *key1, void *key2);

struct AP_DictNode {
    int state;
    AP_Hash hash;
    void *key;
    void *item;
};

struct AP_Dict {
    AP_List *nodes;
    size_t using;
    size_t fullness;
    AP_HashFunc hash;
    AP_DictEqCheck eq_check;

    void *(*get)(AP_Dict *self, void *key);
    int (*set)(AP_Dict *self, void *key, void *item);
    int (*del)(AP_Dict *self, void *key);
    void (*free)(AP_Dict *self);
};

int AP_DictInit(AP_Dict *self, AP_HashFunc hash, AP_DictEqCheck eq_check);
void *AP_DictGet(AP_Dict *self, void *key);
int AP_DictSet(AP_Dict *self, void *key, void *item);
int AP_DictDel(AP_Dict *self, void *key);
void AP_DictFree(AP_Dict *self);
