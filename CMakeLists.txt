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
)

target_include_directories(core_algorithms PUBLIC include)

# Python bindings module
pybind11_add_module(fastdijkstra python/bindings.cpp)
target_link_libraries(fastdijkstra PRIVATE core_algorithms)

# Test executables - CONSOLIDATED TEST SUITE
# The following 4 test executables replace 20+ scattered test files

# 1. Core Functionality Tests (replaces test_main.cpp, test_simple.cpp, test_compilation_check.cpp)
add_executable(test_core_functionality tests/test_core_functionality.cpp)
target_link_libraries(test_core_functionality PRIVATE core_algorithms)

# 2. Comprehensive Test Suite (replaces comprehensive_test_plan.cpp, test_bmssp_*.cpp)
add_executable(test_comprehensive_suite tests/test_comprehensive_suite.cpp)
target_link_libraries(test_comprehensive_suite PRIVATE core_algorithms)

# 3. Edge Cases and Error Handling (replaces debug_*.cpp, test_findpivot*.cpp, etc.)
add_executable(test_edge_cases tests/test_edge_cases.cpp)
target_link_libraries(test_edge_cases PRIVATE core_algorithms)

# 4. Performance and Scalability Tests (replaces safe_performance_test.cpp, test_basecase*.cpp)
add_executable(test_performance tests/test_performance.cpp)
target_link_libraries(test_performance PRIVATE core_algorithms)

# 5. Master Test Runner (orchestrates all test suites)
add_executable(run_all_tests tests/run_all_tests.cpp)
target_link_libraries(run_all_tests PRIVATE core_algorithms)

# Note: Legacy test files have been removed and consolidated into the 4 test suites above.
# This provides better organization, coverage, and maintainability.
