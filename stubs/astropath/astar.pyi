from collections.abc import Sequence
from typing import Optional, List


class Astar:
    def __init__(self, nodes: dict[Sequence[float], Sequence[Sequence[float]]]) -> None:
        ...

    def search(self, start: Sequence[float], end: Sequence[float], /) -> Optional[List[Sequence[float]]]:
        ...
