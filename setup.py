from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="astropath.astar",
            sources=["src/astar.c", "src/AP_heap.c", "src/AP_stack.c", "src/AP_astar.c", "src/AP_list.c", "src/AP_dict.c"]
        ),
    ]
)