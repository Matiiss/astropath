/* 
reference implementation: https://gist.github.com/gerrymanoim/3519567d6afae5361032c032e0cc44f2
from "Python Dictionaries: A confluence of a dozen great ideas - Raymond Hettinger"
see https://www.youtube.com/watch?v=uRKiERJr6yg
*/

#include <stdlib.h>
#include <stdio.h>

#include "./AP_dict.h"

#define TAKEN 0
#define FREE 1
#define DUMMY 2

#define UNSET 0
#define SET 1

AP_List *create_list(size_t new_size);

typedef struct {
    AP_DictNode *node;
    size_t slot;
} SlotTuple;

int AP_DictInit(AP_Dict *self, AP_HashFunc hash, AP_DictEqCheck eq_check) {
    self->nodes = create_list(32);
    if (!self->nodes) {
        return 1;
    }

    self->using = 0;
    self->fullness = 0;
    self->hash = hash;
    self->eq_check = eq_check;

    self->get = &AP_DictGet;
    self->set = &AP_DictSet;
    self->del = &AP_DictDel;
    self->free = &AP_DictFree;

    return 0;
}

int match_keys(AP_Dict *self, AP_DictNode *current, AP_DictNode *target) {
    if (current->key == target->key) {
        return 1;
    }
    if (current->hash != target->hash) {
        return 0;
    }
    return self->eq_check(current->key, target->key);
}

SlotTuple lookup(AP_Dict *self, AP_DictNode *target) {
    SlotTuple tpl;
    int slot_state = UNSET;

    size_t i = target->hash % self->nodes->length;
    AP_Hash perturb = target->hash;

    while (1) {
        AP_DictNode *node = self->nodes->get_at(self->nodes, i);
        if (node->state == FREE) {
            tpl.node = node;
            if (slot_state == UNSET) {
                tpl.slot = i;
            }
            return tpl;
        } else if (node->state == DUMMY) {
            if (slot_state == UNSET) {
                slot_state = SET;
                tpl.slot = i;
            }
        } else if (match_keys(self, node, target)) {
            tpl.node = node;
            tpl.slot = i;
            return tpl;
        }

        i = (5 * i + perturb + 1) % self->nodes->length;
        perturb >>= 5;
    }
}

AP_List *create_list(size_t new_size) {
    AP_List *new_list = malloc(sizeof(AP_List));
    if (!new_list) {
        return NULL;
    }

    if (AP_ListInit(new_list)) {
        return NULL;
    }

    for (size_t i = 0; i < new_size; ++i) {
        AP_DictNode *new_node = malloc(sizeof(AP_DictNode));
        if (!new_node) {
            return NULL;
        }

        new_node->state = FREE;
        new_node->key = NULL;
        if (new_list->append(new_list, (void *)new_node)) {
            return NULL;
        }
    }

    return new_list;
}

int resize(AP_Dict *self, size_t new_size) {
    AP_List *old_list = self->nodes;
    AP_List *new_list = create_list(new_size);
    if (!new_list) {
        return 1;
    }

    self->nodes = new_list;

    for (size_t i = 0; i < old_list->length; ++i) {
        AP_DictNode *node = old_list->get_at(old_list, i);
        if (node->state == TAKEN) {
            SlotTuple tpl = lookup(self, node);
            if (tpl.node->state != FREE) {
                return 1;
            }
            free(new_list->get_at(new_list, tpl.slot));
            if (new_list->set_at(new_list, tpl.slot, (void *)node)) {
                return 1;
            }
        } else {
            free(node);
        }
    }

    old_list->free(old_list);
    free(old_list);

    return 0;
}

void *AP_DictGet(AP_Dict *self, void *key) {
    AP_DictNode node;
    node.hash = self->hash(key);
    node.key = key;

    SlotTuple tpl = lookup(self, &node);
    if (tpl.node->state == FREE) {
        return NULL;
    }

    return tpl.node->item;
}

int AP_DictSet(AP_Dict *self, void *key, void *item) {
    AP_DictNode node;
    node.hash = self->hash(key);
    node.key = key;

    SlotTuple tpl = lookup(self, &node);
    if (tpl.node->state != TAKEN) {
        int state = tpl.node->state;
        tpl.node->state = TAKEN;
        tpl.node->hash = node.hash;
        tpl.node->key = key;
        tpl.node->item = item;
        self->using++;

        if (state == FREE) {
            self->fullness++;
            if (self->fullness * 3 > self->nodes->length * 2) {
                if (resize(self, self->nodes->length * 4)) {
                    return 1;
                }
            }
        }
    } else {
        tpl.node->item = item;
    }
    return 0;
}

int AP_DictDel(AP_Dict *self, void *key) {
    AP_DictNode node;
    node.hash = self->hash(key);
    node.key = key;

    SlotTuple tpl = lookup(self, &node);
    if (tpl.node->state != TAKEN) {
        return 1;
    }

    tpl.node->state = DUMMY;
    self->using--;

    return 0;
}

void AP_DictFree(AP_Dict *self) {
    for (size_t i = 0; i < self->nodes->length; ++i) {
        free(self->nodes->get_at(self->nodes, i));
    }
    self->nodes->free(self->nodes);
    free(self->nodes);
}
