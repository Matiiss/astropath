import collections

import cstar.astar


tile_map = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

tile_size = 32
nodes = collections.defaultdict(list)
offsets = [[-1, -1], [0, -1], [1, -1], [1, 0], [1, 1], [0, 1], [-1, 1], [-1, 0]]

for y, row in enumerate(tile_map):
    for x, cell in enumerate(row):
        if cell != 0:
            continue
        for o_x, o_y in offsets:
            try:
                neighbour_value = tile_map[y + o_y][x + o_x]
                if neighbour_value != 0:
                    continue
            except IndexError:
                continue

            nodes[(tile_size / 2 + x * tile_size, tile_size / 2 + y * tile_size)].append(
                (
                    tile_size / 2 + (x + o_x) * tile_size,
                    tile_size / 2 + (y + o_y) * tile_size,
                )
            )

print(
    cstar.astar.search(
        nodes,
        (tile_size / 2, tile_size / 2),
        (
            tile_size / 2 + len(tile_map[0]) * tile_size,
            tile_size / 2 + len(tile_map) * tile_size,
        ),
    )
)
