```
:module: astropath.astar

:class: Astar(nodes) -> Astar
    search(start, end) -> path | None

    `nodes` is a dictionary contaning all of the nodes as keys and their neighbours as values. The keys are a sequence of two numbers whereas the values are sequences of sequences of two numbers.
    For example,
    ```
    nodes = {(0, 0): [(0, -1), (1, 0), (0, 1), (-1, 0)], ...}
    ```

    :method: search(start, end) -> path | None
        `start` and `end` are sequences of two numbers from `nodes`. Returns the path from `start` to `end` if one can be found, otherwise returns `None`.
```