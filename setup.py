from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_cmake_dir
import pybind11
from setuptools import setup, Extension
import glob

ext_modules = [
    Pybind11Extension(
        "fastdijkstra",
        [
            "python/bindings.cpp",
            "src/Graph.cpp",
            "src/BatchHeap.cpp", 
            "src/Dijkstra.cpp",
            "src/FindPivot.cpp",
            "src/BMSSP.cpp"
        ],
        include_dirs=[
            "include",
            pybind11.get_include()
        ],
        cxx_std=17,
    ),
]

setup(
    name="fastdijkstra",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.6",
)