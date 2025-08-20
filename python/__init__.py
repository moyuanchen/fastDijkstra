"""
fastdijkstra: Fast shortest path algorithms with Python bindings

This package provides high-performance implementations of:
- Dijkstra's algorithm
- BMSSP (Bounded Multi-Source Shortest Path) algorithm achieving O(m log^(2/3) n) complexity
- Graph data structures optimized for shortest path computations

The BMSSP algorithm is the main algorithm that calls the recursive BMSSP procedure with parameters:
- l = ⌈(log n)/t⌉ (initial level)
- S = {s} (single source vertex)
- B = ∞ (infinite bound)

The algorithm uses partial sorting with heap-like data structures and FindPivots procedure
to achieve the optimal O(m log^(2/3) n) running time.

Example usage:
    >>> import fastdijkstra as fd
    >>> 
    >>> # Create a graph with 4 vertices
    >>> graph = fd.Graph(4)
    >>> graph.addEdge(0, 1, 2.0)
    >>> graph.addEdge(0, 2, 4.0)
    >>> graph.addEdge(1, 2, 1.0)
    >>> graph.addEdge(1, 3, 7.0)
    >>> graph.addEdge(2, 3, 3.0)
    >>> 
    >>> # Run Dijkstra from vertex 0
    >>> result = fd.runDijkstra(graph, 0)
    >>> print("Distances:", result.distances)
    >>> print("Predecessors:", result.predecessors)
    >>>
    >>> # Run BMSSP algorithm (main algorithm)
    >>> graph.calcK()
    >>> graph.calcT()
    >>> distances = [float('inf')] * graph.getNumVertices()
    >>> predecessors = [-1] * graph.getNumVertices()
    >>> distances[0] = 0.0
    >>> import math
    >>> level = math.ceil(math.log(graph.getNumVertices()) / graph.getT())
    >>> result = fd.runBMSSP(graph, distances, predecessors, level, float('inf'), [0])
"""

__version__ = "0.1.0"
__author__ = "Mike Chen"
__email__ = ""

# Import the compiled C++ module
try:
    from fastdijkstra import *
    _module_available = True
except ImportError:
    # During development, the module might not be built yet
    _module_available = False

__all__ = [
    'Graph', 'Edge', 'DijkstraResults', 'BaseCaseResults', 'BMSSPResult',
    'runDijkstra', 'runBaseCase', 'runBMSSP'
]
