"""
fastdijkstra: Fast shortest path algorithms with Python bindings

This package provides high-performance implementations of:
- Dijkstra's algorithm
- BMSSP (Bounded Multi-Source Shortest Path) algorithm achieving O(m log^(2/3) n) complexity
- Graph data structures optimized for shortest path computations

The BMSSP algorithm is the main algorithm that calls the recursive BMSSP procedure with parameters:
- l = ⌈(log n)/t⌉ (initial level)
- S = {s} (single source vertex)
- B = ∞ (infinite bound, supports both float('inf') and numpy.inf)

The algorithm uses partial sorting with heap-like data structures and FindPivots procedure
to achieve the optimal O(m log^(2/3) n) running time.

Infinity Support:
- float('inf') and numpy.inf are both supported for infinite bounds
- The C++ implementation properly handles Python infinity values
- Unreachable vertices are represented with infinity values

Example usage:
    >>> import fastdijkstra as fd
    >>> import math
    >>> # Optional: import numpy if available for np.inf support
    >>> # import numpy as np
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
    >>> # Run BMSSP algorithm with mathematical formulation:
    >>> # l = ⌈(log n)/t⌉, S = {s}, B = ∞
    >>> graph.calcK()
    >>> graph.calcT()
    >>> n = graph.getNumVertices()
    >>> t = graph.getT()
    >>> 
    >>> # Initialize arrays for BMSSP
    >>> distances = [float('inf')] * n  # Can also use np.inf
    >>> predecessors = [-1] * n
    >>> distances[0] = 0.0  # Source vertex
    >>> 
    >>> # BMSSP parameters as per the mathematical formulation
    >>> level = math.ceil(math.log(n) / t) if t > 0 else 1  # l = ⌈(log n)/t⌉
    >>> S = [0]  # S = {s} (single source)
    >>> B = float('inf')  # B = ∞ (infinite bound, np.inf also works)
    >>> 
    >>> # Run BMSSP algorithm
    >>> result = fd.runBMSSP(graph, distances, predecessors, level, B, S)
    >>> print("BMSSP completed with new bound:", result.new_bound)
    >>> print("Final distances:", distances)
"""

__version__ = "0.1.0"
__author__ = "Mike"
__email__ = ""

# Import the compiled C++ module
try:
    from ._fastdijkstra import *
    _module_available = True
except ImportError:
    # During development, the module might not be built yet
    _module_available = False

__all__ = [
    'Graph', 'Edge', 'DijkstraResults', 'BaseCaseResults', 'BMSSPResult',
    'runDijkstra', 'runBaseCase', 'runBMSSP'
]
