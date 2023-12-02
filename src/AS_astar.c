#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <Python.h>

#include "./AS_astar.h"
#include "./AS_heap.h"
#include "./AS_stack.h"
#include "./AS_dict.h"

#define UNUSED(x) (void)x

void AS_ANodeFree(AS_ANode *self) {
    free(self->neighbours);
}

double AS_DijkstraHeuristic(AS_ANode *current, AS_ANode *target) {
    UNUSED(current);
    UNUSED(target);
    return 0;
}

double AS_EuclidianDistanceHeuristic(AS_ANode *self, AS_ANode *target) {
    int x1, y1, x2, y2;
    x1 = ((int *)self->data)[0];
    y1 = ((int *)self->data)[1];
    x2 = ((int *)target->data)[0];
    y2 = ((int *)target->data)[1];

    return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
}

inline int AS_AStarLeastDistance(void *primary, void *secondary) {
    return ((AS_ANode *)primary)->tentative_distance < ((AS_ANode *)secondary)->tentative_distance;
}

int AS_AStarReconstructPath(AS_ANode *end, AS_Stack *stack) {
    AS_StackInit(stack);
    AS_ANode *current = end;
    do
    {
        if (stack->push(stack, (void *)current->data2)) {
            return 1;
        }
    } while ((current = current->previous));

    return 0;
}

int AS_AStarSearch(
    AS_ANode *start,
    AS_ANode *target,
    AS_AStarHeuristic heuristic,
    AS_HashFunc hash,
    AS_DictEqCheck eq_check
) {
    if (heuristic == NULL) {
        heuristic = &AS_DijkstraHeuristic;
    }

    start->tentative_distance = heuristic(start, target);
    start->distance = 0;
    start->previous = NULL;

    AS_Heap node_heap_obj;
    AS_Heap *node_heap = &node_heap_obj;
    AS_HeapInit(node_heap, &AS_AStarLeastDistance);
    node_heap->push(node_heap, start);

    AS_Dict pos_dict_obj;
    AS_Dict *pos_dict = &pos_dict_obj;
    AS_DictInit(pos_dict, hash, eq_check);
    pos_dict->set(pos_dict, start->data2, start);

    while (node_heap->length) {
        node_heap->heapify(node_heap);
        AS_ANode *node = node_heap->pop(node_heap);
        node->visited = 1;

        // if (eq_check(node, target)) {
        if (node == target) {
            if (node->previous == NULL) {
                goto failure;
            } else {
                node_heap->free(node_heap);
                pos_dict->free(pos_dict);
                return 0;
            }
        }

        if (node->distance == INFINITY) {
            goto failure;
        }

        for (size_t i = 0; i < node->neighbour_count; ++i) {
            AS_ANode *neighbour = node->neighbours[i];
            
            AS_ANode *dict_entry = pos_dict->get(pos_dict, neighbour->data2);
            if (!dict_entry) {
                neighbour->distance = INFINITY;
                neighbour->tentative_distance = INFINITY;
                neighbour->visited = 0;
                neighbour->previous = NULL;
                pos_dict->set(pos_dict, neighbour->data2, neighbour);
            }

            double distance = node->distance + node->distance_to(node, neighbour);
            if (distance < neighbour->distance) {
                neighbour->previous = node;
                neighbour->distance = distance;
                neighbour->tentative_distance = distance + heuristic(neighbour, target);

                if (neighbour->visited || !dict_entry) {
                    node_heap->push(node_heap, neighbour);
                    neighbour->visited = 0;
                }
            }
        }
    }

failure:
    node_heap->free(node_heap);
    pos_dict->free(pos_dict);
    return 1;
}
