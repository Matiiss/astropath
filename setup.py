from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="cstar.astar",
            sources=["src/astar.c", "src/AS_heap.c", "src/AS_stack.c", "src/AS_astar.c"]
        ),
    ]
)