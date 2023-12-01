#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./AS_astar.h"
#include "./AS_heap.h"
#include "./AS_stack.h"

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

int AS_AStarLeastDistance(void *primary, void *secondary) {
    return ((AS_ANode *)primary)->tentative_distance < ((AS_ANode *)secondary)->tentative_distance;
}

int AS_AStarReconstructPath(AS_ANode *end, AS_Stack *stack) {
    AS_StackInit(stack);
    AS_ANode *current = end;
    do
    {
        if (stack->push(stack, (void *)current->data)) {
            return 1;
        }
    } while (current = current->previous);

    return 0;
}

int AS_AStarSearch(
    AS_ANode *node_array,
    size_t array_size,
    AS_ANode *start,
    AS_ANode *target,
    AS_AStarHeuristic heuristic
) {
    if (heuristic == NULL) {
        heuristic = &AS_DijkstraHeuristic;
    }

    AS_Heap node_heap_obj;
    AS_Heap *node_heap = &node_heap_obj;
    AS_HeapInit(node_heap, &AS_AStarLeastDistance);

    time_t start_t = clock();

    for (size_t i = 0; i < array_size; ++i) {
        AS_ANode *node = &node_array[i];
        node->previous = NULL;
        if (node == start) {
            node->distance = 0;
            node->tentative_distance = heuristic(start, target);
        } else {
            node->distance = INFINITY;
            node->tentative_distance = INFINITY;
        }
        node->visited = 0;
        node_heap->push(node_heap, (void *)node);
    }

    time_t mid = clock();
    printf("%lli\n", (mid - start_t));

    while (node_heap->length) {
        node_heap->heapify(node_heap);
        AS_ANode *node = (AS_ANode *)node_heap->pop(node_heap);
        node->visited = 1;

        // if (eq_check(node, target)) {
        if (node == target) {
            if (node->previous == NULL) {
                goto failure;
            } else {
                node_heap->free(node_heap);
                printf("%lli\n", (clock() - mid));
                return 0;
            }
        }

        if (node->distance == INFINITY) {
            goto failure;
        }

        for (size_t i = 0; i < node->neighbour_count; ++i) {
            AS_ANode *neighbour = node->neighbours[i];

            // if (neighbour->visited) {
            //     continue;
            // }

            double distance = node->distance + node->distance_to(node, neighbour);
            if (distance < neighbour->distance) {
                neighbour->previous = node;
                neighbour->distance = distance;
                neighbour->tentative_distance = distance + heuristic(neighbour, target);

                if (neighbour->visited) {
                    node_heap->push(node_heap, neighbour);
                }
                // int contains = 0;
                // for (size_t i = 0; i < node_heap->length; ++i) {
                //     if (eq_check(neighbour, node_heap->tree[i])) {
                //         contains = 1;
                //         break;
                //     }
                // }
                // if (!contains) {
                //     node_heap->push(node_heap, neighbour);
                // }
            }
        }
    }

failure:
    node_heap->free(node_heap);
    return 1;
}
