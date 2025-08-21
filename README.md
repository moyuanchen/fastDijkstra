# fastDijkstra

A high-performance implementation of the Bounded Multi-Source Shortest Path (BMSSP) algorithm achieving **O(m log^(2/3) n)** complexity - faster than traditional Dijkstra's **O((m+n) log n)** for dense graphs.

## Features

- **BMSSP Algorithm**: Advanced shortest path algorithm with sublinear complexity
- **Classical Dijkstra**: Standard implementation for comparison and verification
- **Python Bindings**: Easy-to-use Python interface via pybind11
- **C++ Core**: High-performance C++17 implementation
- **Comprehensive Testing**: Extensive test suite with performance benchmarks
- **Graph Utilities**: Support for various graph types (sparse, dense, grid, tree, etc.)

## Quick Start

### Installation
```bash
# Simple installation
./install.sh

# Manual installation  
pip install pybind11
pip install -e .
```

### Usage
```python
import fastdijkstra as fd
import math

# Create graph and run BMSSP
graph = fd.Graph(5)
graph.addEdge(0, 1, 2.0)
graph.addEdge(1, 2, 3.0)

# Initialize for BMSSP algorithm
graph.calcK()
graph.calcT()
n = graph.getNumVertices()
distances = [float('inf')] * n
predecessors = [-1] * n
distances[0] = 0.0  # source vertex

# BMSSP algorithm parameters:
# l = ⌈(log n)/t⌉, S = {s}, B = ∞
level = math.ceil(math.log(n) / graph.getT()) if graph.getT() > 0 else 1
S = [0]  # Single source vertex
B = float('inf')  # Infinite bound (np.inf also supported)

# Run algorithms
dijkstra_result = fd.runDijkstra(graph, 0)
bmssp_result = fd.runBMSSP(graph, distances, predecessors, level, B, S)
```

## Project Structure

- `src/` - Core C++ algorithm implementations
- `include/` - Header files for all components  
- `python/` - Python bindings and examples
- `tests/` - Comprehensive test suite
- `docs/` - Documentation and test plans
- `examples/` - Usage examples
- `papers/` - Research papers and references

## Building & Testing

```bash
# Build and run all tests
./build_and_test.sh

# Individual test suites
cd build
./test_core_functionality     # Basic functionality
./test_performance            # Performance benchmarks  
./test_comprehensive_suite    # Algorithm correctness
```

## Key Algorithms

- **BMSSP**: Bounded Multi-Source Shortest Path with O(m log^(2/3) n) complexity
  - Algorithm starts with parameters: l = ⌈(log n)/t⌉, S = {s}, B = ∞
  - Uses recursive decomposition with FindPivots procedure for optimal performance
- **FindPivot**: Efficient pivot selection for graph partitioning
- **BatchHeap**: Custom data structure for batch operations
- **Reference Dijkstra**: Standard implementation for verification

## Performance

The BMSSP algorithm provides significant speedups on dense graphs compared to classical Dijkstra, with comprehensive benchmarks available in the test suite.

## License

See `LICENSE` file for details.
