#include <stdio.h>
#include <stdlib.h>

#include "./AS_astar.h"

void AS_ANodeFree(AS_ANode *self) {
    free(self->neighbours);
}

int AS_AStarLeastDistance(void *primary, void *secondary) {
    return ((AS_ANode *)primary)->distance < ((AS_ANode *)secondary)->distance;
}

int AS_AStarSearch(
    AS_ANode *node_array, 
    size_t array_size, 
    AS_ANode *start, 
    AS_ANode *target, 
    AS_AStarHeuristic *heuristic
) {

}
