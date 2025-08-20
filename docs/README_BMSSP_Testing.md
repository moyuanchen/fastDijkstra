# BMSSP Algorithm Test Framework

This directory contains a comprehensive test framework for validating the Bounded Multi-Source Shortest Path (BMSSP) algorithm implementation.

## Overview

The test framework consists of three main components:

1. **Test Plan Document** (`BMSSP_Test_Plan.md`) - Comprehensive specification for testing BMSSP
2. **Test Framework** (`BMSSPTestFramework.h/cpp`) - C++ framework for automated test generation and execution
3. **Test Executables** - Various test programs for different testing scenarios

## Files Description

### Core Framework Files
- `include/BMSSPTestFramework.h` - Header file defining test framework classes and structures
- `src/BMSSPTestFramework.cpp` - Implementation of the test framework
- `BMSSP_Test_Plan.md` - Detailed test plan specification

### Test Executables
- `tests/test_bmssp_comprehensive.cpp` - Main comprehensive test suite with command-line interface
- `tests/test_bmssp_specialized.cpp` - Specialized test cases for specific scenarios

## Building the Tests

To build the test framework, you'll need to add the following to your CMakeLists.txt:

```cmake
# Add test framework library
add_library(bmssp_test_framework src/BMSSPTestFramework.cpp)
target_include_directories(bmssp_test_framework PUBLIC include)
target_link_libraries(bmssp_test_framework core_algorithms)

# Add comprehensive test executable
add_executable(test_bmssp_comprehensive tests/test_bmssp_comprehensive.cpp)
target_link_libraries(test_bmssp_comprehensive bmssp_test_framework core_algorithms)

# Add specialized test executable
add_executable(test_bmssp_specialized tests/test_bmssp_specialized.cpp)
target_link_libraries(test_bmssp_specialized bmssp_test_framework core_algorithms)
```

Then build:
```bash
cd build
make test_bmssp_comprehensive
make test_bmssp_specialized
```

## Running Tests

### Comprehensive Test Suite

The comprehensive test suite provides multiple testing modes:

```bash
# Run all test suites
./test_bmssp_comprehensive --comprehensive

# Run only correctness tests
./test_bmssp_comprehensive --correctness

# Run only performance tests
./test_bmssp_comprehensive --performance

# Run edge case tests
./test_bmssp_comprehensive --edge-cases

# Run a single custom test
./test_bmssp_comprehensive --single-test

# Show help
./test_bmssp_comprehensive --help
```

### Specialized Test Suite

```bash
# Run all specialized tests
./test_bmssp_specialized
```

## Test Categories

### 1. Correctness Tests

These tests verify that the BMSSP algorithm produces correct results by comparing against reference implementations:

- **Small known graphs**: Hand-crafted graphs with known optimal solutions
- **Multi-source scenarios**: Tests with various source set configurations
- **Reference comparison**: Validation against standard Dijkstra algorithm

### 2. Edge Case Tests

These tests handle boundary conditions and special cases:

- **Single vertex graphs**
- **Disconnected graphs**
- **Empty source sets**
- **Zero bounds**
- **Infinite bounds**
- **Numerical precision edge cases**

### 3. Performance Tests

These tests measure algorithm performance and verify theoretical complexity:

- **Scaling analysis**: Runtime vs. graph size
- **Complexity verification**: Empirical validation of O(m log^(2/3) n) complexity
- **Memory usage analysis**
- **Comparison with baseline algorithms**

### 4. Parameter Variation Tests

These tests validate algorithm behavior with different parameter settings:

- **Various k and t values**
- **Different graph densities**
- **Different source set sizes**
- **Various bound configurations**

## Test Framework Features

### Graph Generation

The framework can generate various types of test graphs:

```cpp
// Random graphs with different densities
GraphType::RANDOM_SPARSE
GraphType::RANDOM_DENSE

// Special structures
GraphType::TREE
GraphType::CYCLE  
GraphType::GRID_2D
GraphType::STAR
GraphType::COMPLETE
GraphType::BIPARTITE
```

### Weight Distributions

Multiple weight distribution patterns are supported:

```cpp
WeightDistribution::UNIFORM         // Uniform real weights
WeightDistribution::EXPONENTIAL     // Exponential distribution
WeightDistribution::INTEGER_SMALL   // Small integer weights
WeightDistribution::INTEGER_LARGE   // Large integer weights
WeightDistribution::UNIT_WEIGHTS    // All weights = 1.0
WeightDistribution::BINARY_WEIGHTS  // Weights ∈ {1.0, 2.0}
```

### Source Set Generation

Various source set generation methods:

```cpp
SourceGenMethod::RANDOM      // Random vertex selection
SourceGenMethod::CLUSTERED   // Spatially clustered sources
SourceGenMethod::SPREAD      // Maximally spread sources
SourceGenMethod::SINGLE_SOURCE // Single source vertex
```

### Verification Methods

The framework provides comprehensive verification:

1. **Correctness verification**: Compare results with reference Dijkstra
2. **Completeness verification**: Ensure all required vertices are included
3. **Bound verification**: Validate B' ≤ B constraint
4. **Size constraint verification**: Check |U| ≥ k × 2^(l×t) when B' < B

## Expected Output

### Successful Test Run

```
=== Running BMSSP Correctness Test Suite ===

Test 1: Small random graph with unit weights
✓ PASSED
  Execution time: 1.23 ms
  Completed vertices: 8
  New bound: 4.00

Test 2: Tree graph test
✓ PASSED
  Execution time: 0.89 ms
  Completed vertices: 6
  New bound: 3.00

=== Test Summary ===
Total tests: 2
Passed: 2
Failed: 0
Success rate: 100.0%
```

### Performance Analysis Output

```
=== Running Performance Analysis ===
Graph Size  Execution Time  Vertices/sec      Status
-------------------------------------------------------
        50        2.34 ms         21368    SUCCESS
       100        5.67 ms         17637    SUCCESS
       200       14.23 ms         14065    SUCCESS
       500       41.89 ms         11940    SUCCESS
```

## Customizing Tests

### Creating Custom Test Cases

```cpp
#include "BMSSPTestFramework.h"

BMSSPTestFramework framework;

// Create custom parameters
TestParameters params;
params.num_vertices = 50;
params.num_edges = 100;
params.graph_type = GraphType::RANDOM_SPARSE;
params.weight_dist = WeightDistribution::UNIFORM;
params.source_method = SourceGenMethod::RANDOM;
params.source_count = 5;
params.bound_type = BoundType::TIGHT;
params.test_name = "My custom test";

// Generate and run test
auto test_case = framework.generateTestCase(params);
auto output = framework.executeBMSSP(test_case);
auto verification = framework.verifyCorrectness(test_case, output);
```

### Adding New Graph Types

To add new graph generation methods:

1. Add new enum value to `GraphType`
2. Implement generation method in `BMSSPTestFramework`
3. Add case to `generateTestCase()` method

### Adding New Verification Criteria

To add new verification methods:

1. Extend `VerificationResult` structure
2. Implement verification logic in `verifyCorrectness()`
3. Update test reporting to include new criteria

## Troubleshooting

### Common Issues

1. **Compilation errors**: Ensure all dependencies (Graph, BMSSP, FindPivot) are properly linked
2. **Test failures**: Check algorithm implementation against expected behavior
3. **Performance issues**: Verify graph parameters (k, t) are reasonable for graph size

### Debug Mode

For detailed debugging information:

```cpp
// Enable verbose output in test framework
#define BMSSP_TEST_DEBUG 1
```

### Memory Issues

For large graphs, monitor memory usage:

```cpp
// Use smaller test sizes if running out of memory
params.num_vertices = 1000; // Instead of 10000
params.num_edges = 2000;    // Keep sparse
```

## Extending the Framework

The test framework is designed to be extensible. Common extensions include:

1. **New graph types**: Add specialized graph structures
2. **New verification methods**: Add domain-specific validation
3. **Performance profiling**: Add detailed timing measurements
4. **Visualization**: Add graph visualization for debugging
5. **Regression testing**: Add historical performance comparison

## Integration with CI/CD

The test framework can be integrated into continuous integration:

```bash
# Basic correctness check
./test_bmssp_comprehensive --correctness

# Performance regression check
./test_bmssp_comprehensive --performance

# Full test suite (for release builds)
./test_bmssp_comprehensive --comprehensive
```

## References

- Original BMSSP paper for algorithm specification
- Test plan document (`BMSSP_Test_Plan.md`) for detailed testing methodology
- Algorithm implementation files (`BMSSP.h`, `BMSSP.cpp`) for implementation details
