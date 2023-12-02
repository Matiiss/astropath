import collections
from timeit import timeit

import cstar.astar


# tile_map = [
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
# ]
tile_map = [[0] * 100 for _ in range(100)]

tile_size = 32
nodes = collections.defaultdict(list)
offsets = [[-1, -1], [0, -1], [1, -1], [1, 0], [1, 1], [0, 1], [-1, 1], [-1, 0]]

for y, row in enumerate(tile_map):
    for x, cell in enumerate(row):
        if cell != 0:
            continue
        for o_x, o_y in offsets:
            if not (0 <= y + o_y < len(tile_map) and 0 <= x + o_x < len(tile_map[0])):
                continue
            neighbour_value = tile_map[y + o_y][x + o_x]
            if neighbour_value != 0:
                continue

            nodes[
                (tile_size / 2 + x * tile_size, tile_size / 2 + y * tile_size)
            ].append(
                (
                    tile_size / 2 + (x + o_x) * tile_size,
                    tile_size / 2 + (y + o_y) * tile_size,
                )
            )

N = 10

# print(timeit("""
# astar = cstar.astar.Astar(nodes)
# """, number=N, globals=globals()))
print("hi")
astar = cstar.astar.Astar(nodes)
print("hi57")
# print(
#     timeit(
#         """
# astar.search(
#     (tile_size / 2, tile_size / 2),
#     (
#         tile_size / 2 + (len(tile_map[0]) - 1) * tile_size,
#         tile_size / 2 + (len(tile_map) - 1) * tile_size,
#     ),
# )
# """,
#         number=N,
#         globals=globals(),
#     )
#     / N
# )

while True:
    astar.search(
    (tile_size / 2, tile_size / 2),
    (
        tile_size / 2 + (len(tile_map[0]) - 1) * tile_size,
        tile_size / 2 + (len(tile_map) - 1) * tile_size,
    ),
    )
