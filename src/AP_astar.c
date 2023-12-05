#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <Python.h>

#include "./AP_astar.h"
#include "./AP_heap.h"
#include "./AP_stack.h"
#include "./AP_dict.h"

#define UNUSED(x) (void)x

void AP_ANodeFree(AP_ANode *self) {
    free(self->neighbours);
}

double AP_DijkstraHeuristic(AP_ANode *current, AP_ANode *target) {
    UNUSED(current);
    UNUSED(target);
    return 0;
}

double AP_EuclidianDistanceHeuristic(AP_ANode *self, AP_ANode *target) {
    int x1, y1, x2, y2;
    x1 = ((int *)self->data)[0];
    y1 = ((int *)self->data)[1];
    x2 = ((int *)target->data)[0];
    y2 = ((int *)target->data)[1];

    return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
}

inline int AP_APtarLeastDistance(void *primary, void *secondary) {
    return ((AP_ANode *)primary)->tentative_distance < ((AP_ANode *)secondary)->tentative_distance;
}

int AP_APtarReconstructPath(AP_ANode *end, AP_Stack *stack) {
    AP_StackInit(stack);
    AP_ANode *current = end;
    do
    {
        if (stack->push(stack, (void *)current->data2)) {
            return 1;
        }
    } while ((current = current->previous));

    return 0;
}

int AP_APtarSearch(
    AP_ANode *start,
    AP_ANode *target,
    AP_APtarHeuristic heuristic,
    AP_HashFunc hash,
    AP_DictEqCheck eq_check
) {
    if (heuristic == NULL) {
        heuristic = &AP_DijkstraHeuristic;
    }

    start->tentative_distance = heuristic(start, target);
    start->distance = 0;
    start->previous = NULL;

    AP_Heap node_heap_obj;
    AP_Heap *node_heap = &node_heap_obj;
    AP_HeapInit(node_heap, &AP_APtarLeastDistance);
    node_heap->push(node_heap, start);

    AP_Dict pos_dict_obj;
    AP_Dict *pos_dict = &pos_dict_obj;
    AP_DictInit(pos_dict, hash, eq_check);
    pos_dict->set(pos_dict, start->data2, start);

    while (node_heap->length) {
        // node_heap->heapify(node_heap);
        AP_ANode *node = node_heap->pop(node_heap);
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
            AP_ANode *neighbour = node->neighbours[i];
            
            AP_ANode *dict_entry = pos_dict->get(pos_dict, neighbour->data2);
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
