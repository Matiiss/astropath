#pragma once

#include <stdlib.h>

#include "./AS_stack.h"

typedef struct AS_ANode AS_ANode;
typedef double (*AS_AStarHeuristic)(AS_ANode *current, AS_ANode *target);
typedef int (*AS_AStarEqualityCheck)(AS_ANode *current, AS_ANode *other);

struct AS_ANode {
    // set on creation
    void *data;
    AS_ANode **neighbours;
    size_t neighbour_count;
    double (*distance_to)(AS_ANode *self, AS_ANode *other);

    // don't worry about these
    AS_ANode *previous;
    double distance;
    double tentative_distance;
    int visited;
};

void AS_ANodeFree(AS_ANode *self);

double AS_DijkstraHeuristic(AS_ANode *current, AS_ANode *target);

int AS_AStarLeastDistance(void *primary, void *secondary);
int AS_AStarReconstructPath(AS_ANode *end, AS_Stack *stack);
int AS_AStarSearch(
    AS_ANode *node_array, 
    size_t array_size, 
    AS_ANode *start, 
    AS_ANode *target,
    AS_AStarHeuristic heuristic
);
