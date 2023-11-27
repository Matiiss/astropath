#include <stdlib.h>

// max int = 2 ** 31 - 1
#define MAX_INT 2147483647

typedef struct AS_DNode AS_DNode;

struct AS_DNode {
    void *data;
    AS_DNode **neighbours;
    size_t neighbour_count;

    AS_DNode *previous;
    float distance;
    int visited;

    float (*distance_to)(AS_DNode *self, AS_DNode *other);
};

void AS_DNodeFree(AS_DNode *self);

int AS_DijkstraLeastDistance(void *primary, void *secondary);
int AS_DijkstraSearch(AS_DNode *node_array, size_t array_size, AS_DNode *start, AS_DNode *target);
