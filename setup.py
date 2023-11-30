from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="cstar.astar",
            sources=["src/astar.c", "src/AS_heap.c", "src/AS_stack.c", "src/AS_astar.c"],
            # include_dirs=["src"],
            # extra_objects=["objects/AS_heap.o", "objects/AS_stack.o", "objects/AS_astar.o"],
        ),
    ]
)