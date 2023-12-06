#pragma once

#include <stdlib.h>

#include "./AP_stack.h"
#include "./AP_dict.h"

typedef struct AP_ANode AP_ANode;
typedef double (*AP_AStarHeuristic)(AP_ANode *current, AP_ANode *target);
typedef int (*AP_AStarEqualityCheck)(AP_ANode *current, AP_ANode *other);

struct AP_ANode {
    // set on creation
    void *data;
    void *data2;
    AP_ANode **neighbours;
    size_t neighbour_count;
    double (*distance_to)(AP_ANode *self, AP_ANode *other);

    // don't worry about these
    AP_ANode *previous;
    double distance;
    double tentative_distance;
    int in_heap;
    int in_used_list;
};

void AP_ANodeFree(AP_ANode *self);

double AP_DijkstraHeuristic(AP_ANode *current, AP_ANode *target);

int AP_AStarLeastDistance(void *primary, void *secondary);
int AP_AStarReconstructPath(AP_ANode *end, AP_Stack *stack);
int AP_AStarSearch(
    AP_ANode *node_arr,
    size_t node_arr_length,
    AP_ANode *start, 
    AP_ANode *target,
    AP_AStarHeuristic heuristic,
    AP_HashFunc hash,
    AP_DictEqCheck eq_check,
    AP_Stack *result_stack
);
