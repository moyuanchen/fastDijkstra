cmake_minimum_required(VERSION 3.12)

project(fastDijkstra)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

add_library(core_algorithms
    src/Graph.cpp
    src/BatchHeap.cpp
    src/Dijkstra.cpp
    src/FindPivot.cpp
    # src/BMSSP.cpp  # Temporarily disabled due to compilation error
)

target_include_directories(core_algorithms PUBLIC include)

pybind11_add_module(fastdijkstra python/bindings.cpp)
target_link_libraries(fastdijkstra PRIVATE core_algorithms)

# Add test executable
add_executable(test_dijkstra test_main.cpp)
target_link_libraries(test_dijkstra PRIVATE core_algorithms)

# Add findpivot test executable
add_executable(test_findpivot test_findpivot.cpp)
target_link_libraries(test_findpivot PRIVATE core_algorithms)

# Add detailed findpivot test executable
add_executable(test_findpivot_detailed test_findpivot_detailed.cpp)
target_link_libraries(test_findpivot_detailed PRIVATE core_algorithms)

# Add edge cases findpivot test executable
add_executable(test_findpivot_edge_cases test_findpivot_edge_cases.cpp)
target_link_libraries(test_findpivot_edge_cases PRIVATE core_algorithms)
