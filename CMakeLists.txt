cmake_minimum_required(VERSION 3.12)

project(fastDijkstra VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

# Core algorithms library
add_library(core_algorithms
    src/Graph.cpp
    src/BatchHeap.cpp
    src/Dijkstra.cpp
    src/FindPivot.cpp
    src/BMSSP.cpp
    src/BMSSPTestFramework.cpp
    src/Debug.cpp
)

target_include_directories(core_algorithms PUBLIC include)

# Python bindings module (commented out - bindings.cpp not found)
# pybind11_add_module(fastdijkstra python/bindings.cpp)
# target_link_libraries(fastdijkstra PRIVATE core_algorithms)

# =============================================================================
# TEST EXECUTABLES - CONSOLIDATED AND ORGANIZED
# =============================================================================

# 1. Core Functionality Tests (basic algorithm verification)
add_executable(test_core_functionality tests/test_core_functionality.cpp)
target_link_libraries(test_core_functionality PRIVATE core_algorithms)

# 2. Comprehensive Test Suite (replaces bmssp_*.cpp files)
add_executable(test_comprehensive_suite tests/test_comprehensive_suite.cpp)
target_link_libraries(test_comprehensive_suite PRIVATE core_algorithms)

# 3. Edge Cases and Error Handling Tests
add_executable(test_edge_cases tests/test_edge_cases.cpp)
target_link_libraries(test_edge_cases PRIVATE core_algorithms)

# 4. Performance and Scalability Tests 
add_executable(test_performance tests/test_performance.cpp)
target_link_libraries(test_performance PRIVATE core_algorithms)

# 5. Large Scale Testing (progressive scaling up to 10K vertices)
add_executable(test_large_scale tests/test_large_scale.cpp)
target_link_libraries(test_large_scale PRIVATE core_algorithms)

# 6. Large Scale Correctness Testing (B=∞ verification)
add_executable(test_correctness_large tests/test_correctness_large.cpp)
target_link_libraries(test_correctness_large PRIVATE core_algorithms)

# 7. Master Test Runner (orchestrates all test suites)
add_executable(run_all_tests tests/run_all_tests.cpp)
target_link_libraries(run_all_tests PRIVATE core_algorithms)

# =============================================================================
# PROJECT STRUCTURE NOTES
# =============================================================================
# 
# Source Files:        src/
# Header Files:        include/
# Test Files:          tests/
# Documentation:       docs/
# Build Directory:     build/
# Examples:            examples/ (currently empty)
# 
# All legacy test files have been consolidated into the organized test suite above.
# The test framework provides comprehensive coverage of:
# - Algorithm correctness
# - Performance characteristics  
# - Edge cases and error handling
# - Large-scale behavior
# - Multi-graph type verification
# 
# =============================================================================
