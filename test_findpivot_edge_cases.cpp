#include <iostream>
#include <vector>
#include <unordered_set>
#include "Graph.h"
#include "FindPivot.h"

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

int main() {
    std::cout << "=== FindPivot Edge Cases and Size Limit Test ===\n" << std::endl;
    
    // Create a graph where the expansion might hit the size limit
    std::cout << "Test: Graph designed to trigger size limit" << std::endl;
    Graph graph(15); // Larger graph
    
    // Create a structure that will expand rapidly
    // Central hub connected to many vertices
    for (int i = 1; i < 15; i++) {
        graph.addEdge(0, i, 1.0);
    }
    // Add some additional connections
    for (int i = 1; i < 10; i++) {
        graph.addEdge(i, i + 5, 1.0);
    }
    
    std::cout << "Graph k parameter: " << graph.getK() << std::endl;
    std::cout << "Number of vertices: " << graph.getNumVertices() << std::endl;
    
    std::cout << "\n--- Test 1: Large expansion (should trigger size limit) ---" << std::endl;
    std::unordered_set<int> S1 = {0};
    std::vector<double> d_hat1(15);
    d_hat1[0] = 0.0;
    for (int i = 1; i < 15; i++) {
        d_hat1[i] = 1.0; // All reachable in 1 step
    }
    double B1 = 10.0; // High bound to allow expansion
    
    printSet(S1, "Frontier set S");
    std::cout << "Upper bound B: " << B1 << std::endl;
    std::cout << "Size limit check: k * |S| = " << graph.getK() << " * " << S1.size() << " = " << graph.getK() * S1.size() << std::endl;
    
    FindPivotResult result1 = findPivots(graph, B1, S1, d_hat1);
    printSet(result1.pivots, "Pivots");
    printSet(result1.nearby, "Nearby vertices");
    
    std::cout << "\n--- Test 2: Multiple sources to increase size limit ---" << std::endl;
    std::unordered_set<int> S2 = {0, 1, 2}; // 3 sources
    std::vector<double> d_hat2(15, 0.5); // Initialize all with small distances
    d_hat2[0] = 0.0;
    d_hat2[1] = 0.0;
    d_hat2[2] = 0.0;
    double B2 = 3.0;
    
    printSet(S2, "Frontier set S");
    std::cout << "Upper bound B: " << B2 << std::endl;
    std::cout << "Size limit check: k * |S| = " << graph.getK() << " * " << S2.size() << " = " << graph.getK() * S2.size() << std::endl;
    
    FindPivotResult result2 = findPivots(graph, B2, S2, d_hat2);
    printSet(result2.pivots, "Pivots");
    printSet(result2.nearby, "Nearby vertices");
    
    std::cout << "\n--- Test 3: Very restrictive bound ---" << std::endl;
    std::unordered_set<int> S3 = {0};
    std::vector<double> d_hat3(15, 1000.0); // Very high initial distances
    d_hat3[0] = 0.0;
    double B3 = 0.5; // Very low bound
    
    printSet(S3, "Frontier set S");
    std::cout << "Upper bound B: " << B3 << std::endl;
    
    FindPivotResult result3 = findPivots(graph, B3, S3, d_hat3);
    printSet(result3.pivots, "Pivots");
    printSet(result3.nearby, "Nearby vertices");
    
    std::cout << "\n--- Test 4: Edge case - empty expansion ---" << std::endl;
    std::unordered_set<int> S4 = {14}; // Start from a leaf node
    std::vector<double> d_hat4(15, 1000.0);
    d_hat4[14] = 0.0;
    double B4 = 0.1; // Very restrictive
    
    printSet(S4, "Frontier set S");
    std::cout << "Upper bound B: " << B4 << std::endl;
    
    FindPivotResult result4 = findPivots(graph, B4, S4, d_hat4);
    printSet(result4.pivots, "Pivots");
    printSet(result4.nearby, "Nearby vertices");
    
    return 0;
}
