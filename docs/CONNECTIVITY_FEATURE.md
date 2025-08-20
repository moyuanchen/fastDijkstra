# Connected Graph Generation Feature

## Overview
This feature adds guaranteed connectivity support to the BMSSPTestFramework, ensuring that generated graphs have paths between all vertices. This is crucial for testing the BMSSP algorithm under realistic conditions where reachability is guaranteed.

## New Components

### 1. Enhanced TestParameters Structure
- `bool ensure_connectivity = false`: Flag to enable connected graph generation
- `bool is_directed = true`: Specifies whether to generate directed or undirected graphs

### 2. New Graph Generation Function
```cpp
Graph generateConnectedGraph(int num_vertices, int num_edges, WeightDistribution dist, bool is_directed = true);
```

#### Algorithm Details

**For Directed Graphs (Strong Connectivity):**
- Creates a directed cycle passing through all vertices: 0→1→2→...→(n-1)→0
- This guarantees a path exists from any vertex to any other vertex
- Adds remaining edges randomly to reach the target edge count

**For Undirected Graphs (Weak Connectivity):**
- Creates a spanning tree using a modified Prim's algorithm
- Simulates undirected edges by adding bidirectional connections
- Adds remaining edges randomly to reach the target edge count

#### Edge Count Requirements
- **Directed graphs**: Minimum `n` edges needed (for the cycle)
- **Undirected graphs**: Minimum `2×(n-1)` edges needed (for bidirectional spanning tree)
- If requested edge count is insufficient, automatically adjusts to minimum required

### 3. Integration with Test Framework

**Updated generateTestCase Method:**
- Checks `params.ensure_connectivity` flag
- Uses `generateConnectedGraph()` when connectivity is required
- Falls back to existing graph generation methods otherwise

**New Test Suites in ComprehensiveTestRunner:**
- `runConnectivityTests()`: Tests directed and undirected connectivity
- `runConnectedTestSeries()`: Tests connected graphs of various sizes
- Integration with size tests to demonstrate connectivity vs. regular graphs

## Usage Examples

### Basic Usage
```cpp
TestParameters params;
params.num_vertices = 20;
params.num_edges = 35;
params.ensure_connectivity = true;  // Enable connectivity guarantee
params.is_directed = true;          // Generate directed strongly connected graph
// ... other parameters
BMSSPTestCase test_case = framework.generateTestCase(params);
```

### Command Line Testing
```bash
# Test connectivity features specifically
./test_comprehensive_suite --connectivity

# Test size variations including connected graphs
./test_comprehensive_suite --size-tests

# Enable debug output to see connectivity algorithm details
./test_comprehensive_suite --connectivity --debug
```

## Benefits

1. **Realistic Testing**: Ensures algorithms are tested on graphs where all vertices are reachable
2. **Paper Compliance**: Aligns with BMSSP paper assumptions about graph connectivity
3. **Controlled Testing**: Allows selective testing of connected vs. potentially disconnected graphs
4. **Flexible Design**: Supports both directed (strongly connected) and undirected (connected) graphs

## Technical Notes

- **Complexity**: O(n + m) where n = vertices, m = edges
- **Memory**: Uses std::set to track existing edges and avoid duplicates
- **Randomization**: Uses the framework's existing RNG for reproducible results
- **Edge Weight Distribution**: Respects the specified weight distribution for all edges

## Debug Output
When debug mode is enabled, the function provides detailed output:
- Connectivity algorithm choice (cycle vs. spanning tree)
- Edge addition progress
- Final graph statistics
- Warnings for insufficient edge counts

This feature enhances the testing framework's capability to generate realistic, connected graphs that better represent real-world scenarios where the BMSSP algorithm would be applied.
