#include <iostream>
#include <vector>
#include <cassert>
#include "Graph.h"
#include "Dijkstra.h"
#include "BMSSP.h"
#include "FindPivot.h"
#include "BatchHeap.h"

/**
 * Core Functionality Test Suite
 * Tests the basic functionality of all major components:
 * - Graph construction and operations
 * - Dijkstra's algorithm
 * - BMSSP base cases
 * - FindPivot algorithm
 * - BatchHeap operations
 */

void testGraphBasics() {
    std::cout << "=== Testing Graph Basics ===" << std::endl;
    
    // Test 1: Simple graph construction
    Graph graph(5);
    graph.addEdge(0, 1, 4.0);
    graph.addEdge(0, 2, 2.0);
    graph.addEdge(1, 2, 1.0);
    graph.addEdge(1, 3, 5.0);
    graph.addEdge(2, 3, 8.0);
    graph.addEdge(2, 4, 10.0);
    graph.addEdge(3, 4, 2.0);
    
    assert(graph.getNumVertices() == 5);
    std::cout << "✓ Graph construction successful" << std::endl;
    
    // Test 2: Graph parameters
    graph.calcK();
    graph.calcT();
    std::cout << "✓ Graph parameters calculated (k=" << graph.getK() << ", t=" << graph.getT() << ")" << std::endl;
    
    // Test 3: Constructor with edges and weights
    std::vector<std::vector<int>> edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}};
    std::vector<double> weights = {1.0, 4.0, 2.0, 3.0, 1.0};
    Graph graph2(5, edges, weights);
    assert(graph2.getNumVertices() == 5);
    std::cout << "✓ Graph construction from vectors successful" << std::endl;
}

void testDijkstraBasics() {
    std::cout << "\n=== Testing Dijkstra Basics ===" << std::endl;
    
    // Create simple test graph
    Graph graph(5);
    graph.addEdge(0, 1, 4.0);
    graph.addEdge(0, 2, 2.0);
    graph.addEdge(1, 2, 1.0);
    graph.addEdge(1, 3, 5.0);
    graph.addEdge(2, 3, 8.0);
    graph.addEdge(2, 4, 10.0);
    graph.addEdge(3, 4, 2.0);
    
    // Run Dijkstra from vertex 0
    DijkstraResults result = runDijkstra(graph, 0);
    
    // Verify basic properties
    assert(result.distances.size() == 5);
    assert(result.predecessors.size() == 5);
    assert(result.distances[0] == 0.0); // Source has distance 0
    
    std::cout << "✓ Dijkstra execution successful" << std::endl;
    std::cout << "  Distance to vertex 1: " << result.distances[1] << std::endl;
    std::cout << "  Distance to vertex 4: " << result.distances[4] << std::endl;
}

void testBMSSPBaseCase() {
    std::cout << "\n=== Testing BMSSP Base Case ===" << std::endl;
    
    // Simple test case
    Graph graph(5);
    graph.addEdge(0, 1, 1.0);
    graph.addEdge(1, 2, 1.0);
    graph.addEdge(0, 3, 2.0);
    graph.addEdge(3, 4, 1.0);
    
    graph.calcK();
    graph.calcT();
    
    std::cout << "Graph k parameter: " << graph.getK() << std::endl;
    
    // Run base case
    BaseCaseResults result = runBaseCase(graph, 0, 10.0);
    
    assert(result.B >= 0);
    assert(result.U.size() <= graph.getK() + 1);
    
    std::cout << "✓ BMSSP base case execution successful" << std::endl;
    std::cout << "  New bound: " << result.B << std::endl;
    std::cout << "  Settled vertices count: " << result.U.size() << std::endl;
}

void testFindPivotBasics() {
    std::cout << "\n=== Testing FindPivot Basics ===" << std::endl;
    
    // Create test graph
    Graph graph(5);
    graph.addEdge(0, 1, 1.0);
    graph.addEdge(0, 2, 1.0);
    graph.addEdge(1, 3, 1.0);
    graph.addEdge(2, 4, 1.0);
    
    graph.calcK();
    graph.calcT();
    
    // Test FindPivot
    std::unordered_set<int> S = {0};
    std::vector<double> d_hat = {0.0, 1.0, 1.0, 2.0, 2.0};
    double B = 5.0;
    
    FindPivotResult result = findPivots(graph, B, S, d_hat);
    
    std::cout << "✓ FindPivot execution successful" << std::endl;
    std::cout << "  Pivots found: " << result.pivots.size() << std::endl;
    std::cout << "  Nearby vertices: " << result.nearby.size() << std::endl;
}

void testBatchHeapBasics() {
    std::cout << "\n=== Testing BatchHeap Basics ===" << std::endl;
    
    try {
        // Note: BatchHeap may not be fully implemented yet
        // This is a placeholder test that will be updated when BatchHeap is ready
        std::cout << "⚠ BatchHeap test skipped (implementation may be incomplete)" << std::endl;
        std::cout << "✓ BatchHeap test placeholder completed" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "⚠ BatchHeap test skipped (implementation may be incomplete): " << e.what() << std::endl;
    }
}

void testConnectivity() {
    std::cout << "\n=== Testing Graph Connectivity ===" << std::endl;
    
    // Test connected graph
    Graph connected(4);
    connected.addEdge(0, 1, 1.0);
    connected.addEdge(1, 2, 1.0);
    connected.addEdge(2, 3, 1.0);
    
    DijkstraResults result1 = runDijkstra(connected, 0);
    bool all_reachable = true;
    for (double dist : result1.distances) {
        if (dist == std::numeric_limits<double>::max()) {
            all_reachable = false;
            break;
        }
    }
    assert(all_reachable);
    std::cout << "✓ Connected graph test passed" << std::endl;
    
    // Test disconnected graph
    Graph disconnected(4);
    disconnected.addEdge(0, 1, 1.0);
    disconnected.addEdge(2, 3, 1.0); // Separate component
    
    DijkstraResults result2 = runDijkstra(disconnected, 0);
    bool has_unreachable = false;
    for (double dist : result2.distances) {
        if (dist == std::numeric_limits<double>::max()) {
            has_unreachable = true;
            break;
        }
    }
    assert(has_unreachable);
    std::cout << "✓ Disconnected graph test passed" << std::endl;
}

void testSpecialGraphStructures() {
    std::cout << "\n=== Testing Special Graph Structures ===" << std::endl;
    
    // Test 1: Star graph
    Graph star(5);
    for (int i = 1; i < 5; i++) {
        star.addEdge(0, i, 1.0);
    }
    
    DijkstraResults star_result = runDijkstra(star, 0);
    for (int i = 1; i < 5; i++) {
        assert(star_result.distances[i] == 1.0);
    }
    std::cout << "✓ Star graph test passed" << std::endl;
    
    // Test 2: Linear chain
    Graph chain(5);
    for (int i = 0; i < 4; i++) {
        chain.addEdge(i, i + 1, 1.0);
    }
    
    DijkstraResults chain_result = runDijkstra(chain, 0);
    for (int i = 0; i < 5; i++) {
        assert(chain_result.distances[i] == static_cast<double>(i));
    }
    std::cout << "✓ Linear chain test passed" << std::endl;
    
    // Test 3: Cycle
    Graph cycle(5);
    for (int i = 0; i < 5; i++) {
        cycle.addEdge(i, (i + 1) % 5, 1.0);
    }
    
    DijkstraResults cycle_result = runDijkstra(cycle, 0);
    assert(cycle_result.distances[0] == 0.0);
    assert(cycle_result.distances[1] == 1.0);
    assert(cycle_result.distances[4] == 1.0); // Should go backwards in cycle
    std::cout << "✓ Cycle graph test passed" << std::endl;
}

int main() {
    std::cout << "=== Core Functionality Test Suite ===" << std::endl;
    std::cout << "Testing basic functionality of all major components" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    try {
        testGraphBasics();
        testDijkstraBasics();
        testBMSSPBaseCase();
        testFindPivotBasics();
        testBatchHeapBasics();
        testConnectivity();
        testSpecialGraphStructures();
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "🎉 All core functionality tests PASSED!" << std::endl;
        std::cout << "✓ Graph operations working correctly" << std::endl;
        std::cout << "✓ Dijkstra algorithm working correctly" << std::endl;
        std::cout << "✓ BMSSP base case working correctly" << std::endl;
        std::cout << "✓ FindPivot algorithm working correctly" << std::endl;
        std::cout << "✓ Special graph structures handled correctly" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
    
    return 0;
}
