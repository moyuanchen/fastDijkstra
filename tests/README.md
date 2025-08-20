# BMSSP Test Suite

This directory contains the consolidated test suite for the Bounded Multi-Source Shortest Path (BMSSP) algorithm implementation. The original 20+ scattered test files have been organized into 4 critical test suites plus a master test runner.

## Test Suite Organization

### 1. Core Functionality Tests (`test_core_functionality.cpp`)
**Purpose**: Validates basic functionality of all major components
- Graph construction and operations
- Dijkstra's algorithm correctness
- BMSSP base case execution
- FindPivot algorithm functionality
- BatchHeap operations
- Special graph structures (star, chain, cycle)

**When to run**: Always - these are the fundamental sanity checks

### 2. Comprehensive Test Suite (`test_comprehensive_suite.cpp`)
**Purpose**: Systematic testing using the BMSSPTestFramework
- Graph size variations (small, medium, large)
- Different graph structures (tree, cycle, star, grid, complete, sparse)
- Edge weight distributions (uniform, exponential, integer, binary)
- Source set variations (single, random, clustered, spread)
- Bound parameter testing (tight, loose, optimal, small)
- Correctness verification against reference implementation

**When to run**: For thorough validation of algorithm correctness

### 3. Edge Cases and Error Handling (`test_edge_cases.cpp`)
**Purpose**: Tests boundary conditions and special scenarios
- Minimal graphs (single vertex, two vertices)
- Disconnected graphs and multiple components
- Zero and very small/large edge weights
- Boundary parameters (zero bound, infinite bound)
- FindPivot edge cases (empty frontier, size limits)
- Self-loops and numerical precision issues

**When to run**: To ensure robustness and proper error handling

### 4. Performance and Scalability Tests (`test_performance.cpp`)
**Purpose**: Analyzes performance characteristics and scaling behavior
- Runtime scaling with graph size
- Performance comparison with baseline Dijkstra
- Graph type performance comparison
- Bound parameter sensitivity analysis
- Stress tests with large graphs
- Algorithmic complexity verification
- Performance report generation (CSV output)

**When to run**: For performance analysis and optimization

### 5. Master Test Runner (`run_all_tests.cpp`)
**Purpose**: Centralized execution of all test suites
- Orchestrates running multiple test executables
- Provides unified command-line interface
- Generates comprehensive test summary
- Supports quick/detailed test modes

## Quick Start

### Build the test suite:
```bash
cd build
make
```

### Run all tests:
```bash
cd tests
./run_all_tests
```

### Run specific test suites:
```bash
# Core functionality only
./run_all_tests --core

# Comprehensive testing
./run_all_tests --comprehensive

# Edge cases
./run_all_tests --edge-cases

# Performance analysis
./run_all_tests --performance

# Quick mode (subset of tests)
./run_all_tests --quick

# Detailed mode (all tests with verbose output)
./run_all_tests --detailed
```

### Run individual test executables:
```bash
# Core tests
./test_core_functionality

# Comprehensive tests with options
./test_comprehensive_suite --correctness
./test_comprehensive_suite --size-tests
./test_comprehensive_suite --all

# Edge case tests
./test_edge_cases

# Performance tests with options
./test_performance --scalability
./test_performance --comparison
./test_performance --stress
./test_performance --all
```

## Test Framework Integration

The test suites leverage the `BMSSPTestFramework` class which provides:

- **Graph Generation**: Random, structured, and special graph types
- **Test Case Creation**: Systematic parameter variation
- **BMSSP Execution**: Algorithm invocation with timing
- **Correctness Verification**: Comparison with reference implementation
- **Performance Measurement**: Detailed timing and throughput analysis

## Consolidated Test Files

### Replaced Files
The following original test files are now consolidated:

**Core functionality**:
- `test_main.cpp` → `test_core_functionality.cpp`
- `test_simple.cpp` → `test_core_functionality.cpp`
- `test_compilation_check.cpp` → `test_core_functionality.cpp`

**Comprehensive testing**:
- `comprehensive_test_plan.cpp` → `test_comprehensive_suite.cpp`
- `test_bmssp_comprehensive.cpp` → `test_comprehensive_suite.cpp`
- `test_bmssp_specialized.cpp` → `test_comprehensive_suite.cpp`

**Edge cases**:
- `debug_basecase.cpp` → `test_edge_cases.cpp`
- `debug_test_framework.cpp` → `test_edge_cases.cpp`
- `debug_verification.cpp` → `test_edge_cases.cpp`
- `test_findpivot.cpp` → `test_edge_cases.cpp`
- `test_findpivot_detailed.cpp` → `test_edge_cases.cpp`
- `test_findpivot_edge_cases.cpp` → `test_edge_cases.cpp`
- `exact_basecase.cpp` → `test_edge_cases.cpp`
- `simplified_test.cpp` → `test_edge_cases.cpp`

**Performance**:
- `safe_performance_test.cpp` → `test_performance.cpp`
- `test_basecase.cpp` → `test_performance.cpp`
- `test_basecase_large.cpp` → `test_performance.cpp`

**BatchHeap specific**:
- `test_batchheap*.cpp` files → Integrated into core functionality tests

### Benefits of Consolidation

1. **Reduced Maintenance**: 4 critical files instead of 20+ scattered files
2. **Clear Organization**: Each test suite has a specific purpose
3. **Better Coverage**: Systematic testing based on the comprehensive test plan
4. **Unified Interface**: Single entry point via master test runner
5. **Consistent Reporting**: Standardized output and error reporting
6. **Framework Integration**: Leverages the BMSSPTestFramework for robustness

## Expected Output

### Successful Test Run
```
BMSSP Algorithm Test Suite
==========================
============================================================
RUNNING: Core Functionality Tests
============================================================
=== Core Functionality Test Suite ===
...
🎉 All core functionality tests PASSED!

============================================================
RUNNING: Comprehensive Algorithm Tests
============================================================
=== COMPREHENSIVE BMSSP TEST SUITE ===
...
🎉 ALL TESTS PASSED! Algorithm implementation verified.

============================================================
FINAL TEST SUMMARY
============================================================
Core Functionality              : ✅ PASSED
Comprehensive Tests              : ✅ PASSED
Edge Cases                       : ✅ PASSED
Performance Tests                : ✅ PASSED
------------------------------------------------------------
Test Suites: 4/4 passed

🎉 ALL TEST SUITES PASSED! 🎉
🚀 BMSSP implementation is ready for use!
```

This consolidated test suite provides comprehensive validation while being much easier to maintain and use than the original scattered collection of test files.
