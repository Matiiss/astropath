from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="cstar.astar",
            sources=["src/astar.c"],
            extra_objects=["objects/AS_heap.o"]  # , "objects/AS_stack.o", "objects/AS_astar.o"]
        ),
    ]
)