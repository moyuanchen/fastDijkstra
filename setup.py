#!/usr/bin/env python3

from setuptools import setup, Extension, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_cmake_dir
import pybind11
import os

# Define the extension module - this will be built into the fastdijkstra package
ext_modules = [
    Pybind11Extension(
        "fastdijkstra._fastdijkstra",  # Build as a submodule
        [
            "python/bindings.cpp",
            "src/Graph.cpp",
            "src/BatchHeap.cpp", 
            "src/Dijkstra.cpp",
            "src/FindPivot.cpp",
            "src/BMSSP.cpp",
            "src/BMSSPTestFramework.cpp",
            "src/Debug.cpp",
        ],
        include_dirs=[
            "include",
            # Path to pybind11 headers
            pybind11.get_include(),
        ],
        language='c++',
        cxx_std=17,
    ),
]

# Read README for long description
def read_readme():
    try:
        with open("README.md", "r", encoding="utf-8") as f:
            return f.read()
    except FileNotFoundError:
        return "Fast Dijkstra and BMSSP algorithms for shortest path computation"

setup(
    name="fastdijkstra",
    version="0.1.0",
    author="Your Name",
    author_email="your.email@example.com",
    description="Fast Dijkstra and BMSSP algorithms for shortest path computation",
    long_description=read_readme(),
    long_description_content_type="text/markdown",
    packages=find_packages(),
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.6",
    install_requires=[
        "pybind11>=2.6.0",
    ],
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Topic :: Scientific/Engineering",
        "Topic :: Software Development :: Libraries :: Python Modules",
    ],
    keywords="dijkstra shortest-path graph-algorithms bmssp",
    url="https://github.com/moyuanchen/fastdijkstra",
)
