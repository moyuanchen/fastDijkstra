# Installation Instructions

## Simple Installation

The simplest way to install fastdijkstra is using the provided installation script:

```bash
./install.sh
```

This script will:
1. Install pybind11 if not already installed
2. Install fastdijkstra in development mode
3. Test the installation

## Manual Installation

### Option 1: Development Installation (Recommended)
```bash
pip install pybind11
pip install -e .
```

### Option 2: Regular Installation
```bash
pip install pybind11
pip install .
```

### Option 3: Using pyproject.toml (Modern Python)
```bash
pip install .
```

## Requirements

- Python 3.6 or higher
- C++ compiler with C++17 support
- pybind11 (automatically installed)

## Testing Installation

After installation, test with:
```python
import fastdijkstra
print("fastdijkstra installed successfully!")

# Quick test
graph = fastdijkstra.Graph(3)
graph.addEdge(0, 1, 1.0)
graph.addEdge(1, 2, 2.0)
result = fastdijkstra.runDijkstra(graph, 0)
print("Distances:", result.distances)
```

## Troubleshooting

If you encounter issues:

1. **Compiler errors**: Ensure you have a C++17 compatible compiler
2. **pybind11 errors**: Update pybind11 with `pip install --upgrade pybind11`
3. **Import errors**: Try `pip install -e .` for development mode installation
