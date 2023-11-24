#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "./AS_dijkstra.h"
#include "./AS_heap.c"

float EuclidianDistance(AS_DNode *self, AS_DNode *other);

int main() {
    int map[10][10][2] = {
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}},
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1}},
        {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}},
        {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}},
        {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {8, 4}, {9, 4}},
        {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}, {9, 5}},
        {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {7, 6}, {8, 6}, {9, 6}},
        {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7}, {8, 7}, {9, 7}},
        {{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8}, {9, 8}},
        {{0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}}
    };

    int offsets[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    AS_DNode nodes[10 * 10];

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            AS_DNode node;
            node.data = (void *)map[y][x];
            node.neighbours = malloc(sizeof(AS_DNode *) * 8);
            node.distance_to = &EuclidianDistance;
            nodes[y * 10 + x] = node;
        }
    }

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            AS_DNode *node = &nodes[y * 10 + x];
            int count = 0;

            for (int i = 0; i < 8; ++i) {
                int o_x, o_y, n_x, n_y;
                o_x = offsets[i][0];
                o_y = offsets[i][1];
                n_x = x + o_x;
                n_y = y + o_y;

                if (0 <= n_x && n_x < 10 && 0 <= n_y && n_y < 10) {
                    node->neighbours[count] = &nodes[n_y * 10 + n_x];
                    count++;
                }
            }

            node->neighbour_count = count;
            if (count < 8) {
                node->neighbours = realloc(node->neighbours, sizeof(AS_DNode *) * count);
            }
        }
    }

    printf("%i\n", AS_DijkstraSearch(nodes, 10 * 10, &nodes[0], &nodes[2 * 10 + 3]));

    AS_DNode *current = &nodes[2 * 10 + 3];
    while (1) {
        int *coords = (int *)current->data;
        printf("%i, %i\n", coords[0], coords[1]);

        current = current->previous;
        if (!current) {
            break;
        }
    }

    return 0;
}

float EuclidianDistance(AS_DNode *self, AS_DNode *other) {
    int x1, y1, x2, y2;
    x1 = ((int *)self->data)[0];
    y1 = ((int *)self->data)[1];
    x2 = ((int *)other->data)[0];
    y2 = ((int *)other->data)[1];

    return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
}

int AS_DijkstraLeastDistance(void *primary, void *secondary) {
    return ((AS_DNode *)primary)->distance < ((AS_DNode *)secondary)->distance;
}

int AS_DijkstraSearch(AS_DNode *node_array, size_t array_size, AS_DNode *start, AS_DNode *target) {
    AS_Heap node_heap_obj;
    AS_Heap *node_heap = &node_heap_obj;
    AS_HeapInit(node_heap, &AS_DijkstraLeastDistance);

    for (size_t i = 0; i < array_size; ++i) {
        AS_DNode *node = &node_array[i];
        node->previous = NULL;
        if (node == start) {
            node->distance = 0;
        } else {
            node->distance = MAX_INT;
        }
        node_heap->push(node_heap, (void *)node);
    }

    while (node_heap->length) {
        node_heap->heapify(node_heap);
        AS_DNode *node = (AS_DNode *)node_heap->pop(node_heap);

        if (node == target) {
            return 1;
        }

        for (size_t i = 0; i < node->neighbour_count; ++i) {
            AS_DNode *neighbour = node->neighbours[i];
            int visited = 0;
            for (size_t j = 0; j < node_heap->length; ++j) {
                if (neighbour == (AS_DNode *)&node_heap->tree[j]) {
                    visited = 1;
                }
            }

            if (visited) {
                continue;
            }

            float distance = node->distance + node->distance_to(node, neighbour);
            if (distance < neighbour->distance) {
                neighbour->distance = distance;
                neighbour->previous = node;
            }
        }
    }

    return 0;
}
