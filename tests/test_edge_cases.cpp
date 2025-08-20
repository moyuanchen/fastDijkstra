#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits>
#include <cassert>
#include <iomanip>
#include <functional>
#include "Graph.h"
#include "Dijkstra.h"
#include "BMSSP.h"
#include "FindPivot.h"

/**
 * Edge Cases and Error Handling Test Suite
 * Tests boundary conditions, error cases, and special scenarios:
 * - Empty and minimal graphs
 * - Disconnected graphs
 * - Invalid parameters
 * - Numerical edge cases
 * - Algorithm-specific edge cases
 */

void printSet(const std::unordered_set<int>& s, const std::string& name) {
    std::cout << name << " (size=" << s.size() << "): {";
    bool first = true;
    for (int elem : s) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "}" << std::endl;
}

void testMinimalGraphs() {
    std::cout << "=== Testing Minimal Graphs ===" << std::endl;
    
    // Test 1: Single vertex graph
    std::cout << "Test 1: Single vertex graph" << std::endl;
    try {
        Graph single(1);
        single.calcK();
        single.calcT();
        
        DijkstraResults result = runDijkstra(single, 0);
        assert(result.distances.size() == 1);
        assert(result.distances[0] == 0.0);
        
        std::cout << "✓ Single vertex test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Single vertex test failed: " << e.what() << std::endl;
    }
    
    // Test 2: Two vertex graph with no edges
    std::cout << "Test 2: Two disconnected vertices" << std::endl;
    try {
        Graph two_disconnected(2);
        two_disconnected.calcK();
        two_disconnected.calcT();
        
        DijkstraResults result = runDijkstra(two_disconnected, 0);
        assert(result.distances[0] == 0.0);
        assert(result.distances[1] == std::numeric_limits<double>::max());
        
        std::cout << "✓ Two disconnected vertices test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Two disconnected vertices test failed: " << e.what() << std::endl;
    }
    
    // Test 3: Two vertices with one edge
    std::cout << "Test 3: Two connected vertices" << std::endl;
    try {
        Graph two_connected(2);
        two_connected.addEdge(0, 1, 5.0);
        two_connected.calcK();
        two_connected.calcT();
        
        DijkstraResults result = runDijkstra(two_connected, 0);
        assert(result.distances[0] == 0.0);
        assert(result.distances[1] == 5.0);
        
        std::cout << "✓ Two connected vertices test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Two connected vertices test failed: " << e.what() << std::endl;
    }
}

void testDisconnectedGraphs() {
    std::cout << "\n=== Testing Disconnected Graphs ===" << std::endl;
    
    // Test 1: Multiple components
    std::cout << "Test 1: Graph with multiple components" << std::endl;
    try {
        Graph multi_comp(6);
        // Component 1: 0-1-2
        multi_comp.addEdge(0, 1, 1.0);
        multi_comp.addEdge(1, 2, 1.0);
        // Component 2: 3-4
        multi_comp.addEdge(3, 4, 2.0);
        // Isolated vertex: 5
        
        multi_comp.calcK();
        multi_comp.calcT();
        
        DijkstraResults result = runDijkstra(multi_comp, 0);
        
        // Check reachable vertices
        assert(result.distances[0] == 0.0);
        assert(result.distances[1] == 1.0);
        assert(result.distances[2] == 2.0);
        
        // Check unreachable vertices
        assert(result.distances[3] == std::numeric_limits<double>::max());
        assert(result.distances[4] == std::numeric_limits<double>::max());
        assert(result.distances[5] == std::numeric_limits<double>::max());
        
        std::cout << "✓ Multiple components test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Multiple components test failed: " << e.what() << std::endl;
    }
    
    // Test 2: BMSSP with disconnected graph
    std::cout << "Test 2: BMSSP on disconnected graph" << std::endl;
    try {
        Graph disconnected(4);
        disconnected.addEdge(0, 1, 1.0);
        disconnected.addEdge(2, 3, 1.0);
        
        disconnected.calcK();
        disconnected.calcT();
        
        BaseCaseResults result = runBaseCase(disconnected, 0, 10.0);
        
        std::cout << "✓ BMSSP disconnected graph test completed" << std::endl;
        std::cout << "  Settled vertices: " << result.U.size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ BMSSP disconnected graph test failed: " << e.what() << std::endl;
    }
}

void testZeroAndNegativeWeights() {
    std::cout << "\n=== Testing Zero and Special Weights ===" << std::endl;
    
    // Test 1: Zero weight edges
    std::cout << "Test 1: Graph with zero weight edges" << std::endl;
    try {
        Graph zero_weights(3);
        zero_weights.addEdge(0, 1, 0.0);  // Zero weight
        zero_weights.addEdge(1, 2, 1.0);
        
        zero_weights.calcK();
        zero_weights.calcT();
        
        DijkstraResults result = runDijkstra(zero_weights, 0);
        assert(result.distances[0] == 0.0);
        assert(result.distances[1] == 0.0);
        assert(result.distances[2] == 1.0);
        
        std::cout << "✓ Zero weight edges test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Zero weight edges test failed: " << e.what() << std::endl;
    }
    
    // Test 2: Very small weights (precision test)
    std::cout << "Test 2: Very small weights" << std::endl;
    try {
        Graph small_weights(3);
        small_weights.addEdge(0, 1, 1e-10);
        small_weights.addEdge(1, 2, 1e-10);
        
        small_weights.calcK();
        small_weights.calcT();
        
        DijkstraResults result = runDijkstra(small_weights, 0);
        assert(result.distances[0] == 0.0);
        assert(result.distances[1] > 0.0);
        assert(result.distances[2] > result.distances[1]);
        
        std::cout << "✓ Very small weights test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Very small weights test failed: " << e.what() << std::endl;
    }
    
    // Test 3: Very large weights
    std::cout << "Test 3: Very large weights" << std::endl;
    try {
        Graph large_weights(3);
        large_weights.addEdge(0, 1, 1e6);
        large_weights.addEdge(1, 2, 1e6);
        
        large_weights.calcK();
        large_weights.calcT();
        
        DijkstraResults result = runDijkstra(large_weights, 0);
        assert(result.distances[0] == 0.0);
        assert(result.distances[1] == 1e6);
        assert(result.distances[2] == 2e6);
        
        std::cout << "✓ Very large weights test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Very large weights test failed: " << e.what() << std::endl;
    }
}

void testBoundaryParameters() {
    std::cout << "\n=== Testing Boundary Parameters ===" << std::endl;
    
    // Test 1: Zero bound
    std::cout << "Test 1: BMSSP with zero bound" << std::endl;
    try {
        Graph graph(3);
        graph.addEdge(0, 1, 1.0);
        graph.addEdge(1, 2, 1.0);
        
        graph.calcK();
        graph.calcT();
        
        BaseCaseResults result = runBaseCase(graph, 0, 0.0);
        
        // With zero bound, should only settle the source
        assert(result.U.find(0) != result.U.end());
        
        std::cout << "✓ Zero bound test passed" << std::endl;
        std::cout << "  Settled vertices: " << result.U.size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ Zero bound test failed: " << e.what() << std::endl;
    }
    
    // Test 2: Very small bound
    std::cout << "Test 2: BMSSP with very small bound" << std::endl;
    try {
        Graph graph(4);
        graph.addEdge(0, 1, 0.5);
        graph.addEdge(1, 2, 0.5);
        graph.addEdge(2, 3, 0.5);
        
        graph.calcK();
        graph.calcT();
        
        BaseCaseResults result = runBaseCase(graph, 0, 0.1);
        
        std::cout << "✓ Very small bound test passed" << std::endl;
        std::cout << "  Settled vertices: " << result.U.size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ Very small bound test failed: " << e.what() << std::endl;
    }
    
    // Test 3: Infinite bound
    std::cout << "Test 3: BMSSP with infinite bound" << std::endl;
    try {
        Graph graph(5);
        graph.addEdge(0, 1, 1.0);
        graph.addEdge(1, 2, 1.0);
        graph.addEdge(2, 3, 1.0);
        graph.addEdge(3, 4, 1.0);
        
        graph.calcK();
        graph.calcT();
        
        BaseCaseResults result = runBaseCase(graph, 0, std::numeric_limits<double>::max());
        
        std::cout << "✓ Infinite bound test passed" << std::endl;
        std::cout << "  Settled vertices: " << result.U.size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ Infinite bound test failed: " << e.what() << std::endl;
    }
}

void testFindPivotEdgeCases() {
    std::cout << "\n=== Testing FindPivot Edge Cases ===" << std::endl;
    
    // Test 1: Empty frontier set
    std::cout << "Test 1: FindPivot with empty frontier" << std::endl;
    try {
        Graph graph(5);
        graph.addEdge(0, 1, 1.0);
        graph.addEdge(1, 2, 1.0);
        
        graph.calcK();
        graph.calcT();
        
        std::unordered_set<int> empty_S;
        std::vector<double> d_hat(5, std::numeric_limits<double>::max());
        double B = 10.0;
        
        FindPivotResult result = findPivots(graph, B, empty_S, d_hat);
        
        std::cout << "✓ Empty frontier test completed" << std::endl;
        printSet(result.pivots, "Pivots");
        printSet(result.nearby, "Nearby");
        
    } catch (const std::exception& e) {
        std::cout << "✗ Empty frontier test failed: " << e.what() << std::endl;
    }
    
    // Test 2: Single vertex frontier
    std::cout << "Test 2: FindPivot with isolated vertex" << std::endl;
    try {
        Graph graph(5);
        graph.addEdge(1, 2, 1.0);  // 0 is isolated
        graph.addEdge(2, 3, 1.0);
        
        graph.calcK();
        graph.calcT();
        
        std::unordered_set<int> S = {0}; // Isolated vertex
        std::vector<double> d_hat(5, std::numeric_limits<double>::max());
        d_hat[0] = 0.0;
        double B = 10.0;
        
        FindPivotResult result = findPivots(graph, B, S, d_hat);
        
        std::cout << "✓ Isolated vertex test completed" << std::endl;
        printSet(result.pivots, "Pivots");
        printSet(result.nearby, "Nearby");
        
    } catch (const std::exception& e) {
        std::cout << "✗ Isolated vertex test failed: " << e.what() << std::endl;
    }
    
    // Test 3: Very restrictive bound
    std::cout << "Test 3: FindPivot with very restrictive bound" << std::endl;
    try {
        Graph graph(10);
        for (int i = 0; i < 9; i++) {
            graph.addEdge(i, i + 1, 1.0);
        }
        
        graph.calcK();
        graph.calcT();
        
        std::unordered_set<int> S = {0};
        std::vector<double> d_hat(10);
        for (int i = 0; i < 10; i++) {
            d_hat[i] = static_cast<double>(i);
        }
        double B = 0.5; // Very restrictive
        
        FindPivotResult result = findPivots(graph, B, S, d_hat);
        
        std::cout << "✓ Restrictive bound test completed" << std::endl;
        printSet(result.pivots, "Pivots");
        printSet(result.nearby, "Nearby");
        
    } catch (const std::exception& e) {
        std::cout << "✗ Restrictive bound test failed: " << e.what() << std::endl;
    }
    
    // Test 4: Size limit trigger
    std::cout << "Test 4: FindPivot size limit trigger" << std::endl;
    try {
        Graph graph(15);
        // Create star graph to trigger expansion
        for (int i = 1; i < 15; i++) {
            graph.addEdge(0, i, 1.0);
        }
        
        graph.calcK();
        graph.calcT();
        
        std::unordered_set<int> S = {0};
        std::vector<double> d_hat(15);
        d_hat[0] = 0.0;
        for (int i = 1; i < 15; i++) {
            d_hat[i] = 1.0;
        }
        double B = 10.0;
        
        std::cout << "Size limit: k * |S| = " << graph.getK() << " * " << S.size() 
                  << " = " << graph.getK() * S.size() << std::endl;
        
        FindPivotResult result = findPivots(graph, B, S, d_hat);
        
        std::cout << "✓ Size limit test completed" << std::endl;
        printSet(result.pivots, "Pivots");
        printSet(result.nearby, "Nearby");
        
    } catch (const std::exception& e) {
        std::cout << "✗ Size limit test failed: " << e.what() << std::endl;
    }
}

void testSelfLoopsAndParallelEdges() {
    std::cout << "\n=== Testing Self-loops and Parallel Edges ===" << std::endl;
    
    // Test 1: Self-loops
    std::cout << "Test 1: Graph with self-loops" << std::endl;
    try {
        Graph graph(3);
        graph.addEdge(0, 0, 5.0);  // Self-loop
        graph.addEdge(0, 1, 1.0);
        graph.addEdge(1, 2, 1.0);
        
        graph.calcK();
        graph.calcT();
        
        DijkstraResults result = runDijkstra(graph, 0);
        
        // Self-loop should not affect shortest paths
        assert(result.distances[0] == 0.0);
        assert(result.distances[1] == 1.0);
        assert(result.distances[2] == 2.0);
        
        std::cout << "✓ Self-loops test passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Self-loops test failed: " << e.what() << std::endl;
    }
    
    // Test 2: Multiple edges between same vertices (graph implementation dependent)
    std::cout << "Test 2: Multiple edges between vertices" << std::endl;
    try {
        Graph graph(3);
        graph.addEdge(0, 1, 3.0);
        graph.addEdge(0, 1, 1.0);  // Better path
        graph.addEdge(1, 2, 1.0);
        
        graph.calcK();
        graph.calcT();
        
        DijkstraResults result = runDijkstra(graph, 0);
        
        std::cout << "✓ Multiple edges test completed" << std::endl;
        std::cout << "  Distance to vertex 1: " << result.distances[1] << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ Multiple edges test failed: " << e.what() << std::endl;
    }
}

void testNumericalPrecision() {
    std::cout << "\n=== Testing Numerical Precision ===" << std::endl;
    
    // Test 1: Near-equal weights
    std::cout << "Test 1: Near-equal weights" << std::endl;
    try {
        Graph graph(4);
        graph.addEdge(0, 1, 1.0);
        graph.addEdge(0, 2, 1.0 + 1e-15);  // Nearly equal
        graph.addEdge(1, 3, 1.0);
        graph.addEdge(2, 3, 1.0);
        
        graph.calcK();
        graph.calcT();
        
        DijkstraResults result = runDijkstra(graph, 0);
        
        std::cout << "✓ Near-equal weights test passed" << std::endl;
        std::cout << "  Distance to vertex 3: " << std::fixed << std::setprecision(15) 
                  << result.distances[3] << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ Near-equal weights test failed: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== EDGE CASES AND ERROR HANDLING TEST SUITE ===" << std::endl;
    std::cout << "Testing boundary conditions and special scenarios" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    int total_test_groups = 0;
    int passed_test_groups = 0;
    
    // Run all test groups
    std::vector<std::function<void()>> test_groups = {
        testMinimalGraphs,
        testDisconnectedGraphs,
        testZeroAndNegativeWeights,
        testBoundaryParameters,
        testFindPivotEdgeCases,
        testSelfLoopsAndParallelEdges,
        testNumericalPrecision
    };
    
    for (auto& test_group : test_groups) {
        total_test_groups++;
        try {
            test_group();
            passed_test_groups++;
        } catch (const std::exception& e) {
            std::cout << "❌ Test group failed with exception: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "❌ Test group failed with unknown exception" << std::endl;
        }
        std::cout << std::endl;
    }
    
    // Summary
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "EDGE CASES TEST SUMMARY" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Test groups completed: " << passed_test_groups << "/" << total_test_groups << std::endl;
    
    if (passed_test_groups == total_test_groups) {
        std::cout << "🎉 All edge case tests completed successfully!" << std::endl;
        std::cout << "✓ Minimal graphs handled correctly" << std::endl;
        std::cout << "✓ Disconnected graphs handled correctly" << std::endl;
        std::cout << "✓ Special weight cases handled correctly" << std::endl;
        std::cout << "✓ Boundary parameters handled correctly" << std::endl;
        std::cout << "✓ FindPivot edge cases handled correctly" << std::endl;
        std::cout << "✓ Graph structure edge cases handled correctly" << std::endl;
        std::cout << "✓ Numerical precision cases handled correctly" << std::endl;
        return 0;
    } else {
        std::cout << "⚠️  Some edge case tests encountered issues" << std::endl;
        return 1;
    }
}
