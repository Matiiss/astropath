#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "./AS_astar.h"
#include "./AS_heap.h"
#include "./AS_stack.h"

#define UNUSED(x) (void)x
// #define N 100

// float EuclidianDistance(AS_ANode *self, AS_ANode *other);

// int main() {
//     int map[10][10][2] = {
//         {{0, 0}, {1, 0}, {2, 0}, {N, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}},
//         {{0, 1}, {N, 1}, {N, 1}, {N, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1}},
//         {{0, 2}, {N, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}},
//         {{N, 3}, {N, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}},
//         {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {8, 4}, {9, 4}},
//         {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}, {9, 5}},
//         {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 6}, {9, 6}},
//         {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7}, {8, 7}, {9, 7}},
//         {{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8}, {9, 8}},
//         {{0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}}
//     };

//     int offsets[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
//     AS_ANode nodes[10 * 10];
//     int node_count = 0;

//     for (int y = 0; y < 10; ++y) {
//         for (int x = 0; x < 10; ++x) {
//             if (map[y][x][0] == N || map[y][x][1] == N) {
//                 continue;
//             }
//             AS_ANode node;
//             node.data = (void *)map[y][x];
//             node.neighbours = malloc(sizeof(AS_ANode *) * 8);
//             node.distance_to = &EuclidianDistance;
//             nodes[node_count] = node;
//             node_count++;
//         }
//     }

//     for (int i = 0; i < node_count; ++i) {
//         AS_ANode *node = &nodes[i];
//         int count = 0;

//         for (int i = 0; i < 8; ++i) {
//             int o_x, o_y, n_x, n_y;
//             o_x = offsets[i][0];
//             o_y = offsets[i][1];
//             n_x = o_x + ((int *)node->data)[0];
//             n_y = o_y + ((int *)node->data)[1];

//             if (0 <= n_x && n_x < 10 && 0 <= n_y && n_y < 10) {
//                 for (int i = 0; i < node_count; ++i) {
//                     AS_ANode *neighbour = &nodes[i];
//                     if (((int *)neighbour->data)[0] == n_x && ((int *)neighbour->data)[1] == n_y) {
//                         node->neighbours[count] = neighbour;
//                         count++;
//                         break;
//                     }
//                 }
//             }
//         }

//         node->neighbour_count = count;
//         if (count < 8) {
//             node->neighbours = realloc(node->neighbours, sizeof(AS_ANode *) * count);
//         }
//     }

//     printf("%i\n", AS_AStarSearch(nodes, node_count, &nodes[0], &nodes[node_count - 1], NULL));
//     for (int i = 0; i < node_count; ++i) {
//         AS_ANodeFree(&nodes[i]);
//     }

//     AS_ANode *current = &nodes[node_count - 1];
//     while (1) {
//         int *coords = (int *)current->data;
//         printf("%i, %i\n", coords[0], coords[1]);

//         current = current->previous;
//         if (!current) {
//             break;
//         }
//     }

//     return 0;
// }

// double EuclidianDistance(AS_ANode *self, AS_ANode *other) {
//     int x1, y1, x2, y2;
//     x1 = ((int *)self->data)[0];
//     y1 = ((int *)self->data)[1];
//     x2 = ((int *)other->data)[0];
//     y2 = ((int *)other->data)[1];

//     return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
// }

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
    AS_AStarEqualityCheck eq_check,
    AS_AStarHeuristic heuristic
) {
    if (heuristic == NULL) {
        heuristic = &AS_DijkstraHeuristic;
    }

    AS_Heap node_heap_obj;
    AS_Heap *node_heap = &node_heap_obj;
    AS_HeapInit(node_heap, &AS_AStarLeastDistance);

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
