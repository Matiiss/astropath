#include <stdlib.h>

typedef struct AS_ANode AS_ANode;
typedef float (*AS_AStarHeuristic)(AS_ANode *current, AS_ANode *target);

struct AS_ANode {
    void *data;
    AS_ANode **neighbours;
    size_t neighbour_count;

    AS_ANode *previous;
    float distance;
    float tentative_distance;
    int visited;

    float (*distance_to)(AS_ANode *self, AS_ANode *other);
};

void AS_ANodeFree(AS_ANode *self);

float AS_DijkstraHeuristic(AS_ANode *current, AS_ANode *target);

int AS_AStarLeastDistance(void *primary, void *secondary);
int AS_AStarSearch(
    AS_ANode *node_array, 
    size_t array_size, 
    AS_ANode *start, 
    AS_ANode *target, 
    AS_AStarHeuristic heuristic
);
