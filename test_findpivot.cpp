#include <iostream>
#include <vector>
#include <unordered_set>
#include "Graph.h"
#include "FindPivot.h"

void printSet(const std::unordered_set<int>& s, const std::string& name) {
    std::cout << name << ": {";
    bool first = true;
    for (int elem : s) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "}" << std::endl;
}

void printDistances(const std::vector<double>& distances, const std::string& name) {
    std::cout << name << ": [";
    for (size_t i = 0; i < distances.size(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << distances[i];
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "=== Testing FindPivot Implementation ===\n" << std::endl;
    
    // Test 1: Simple linear graph
    std::cout << "Test 1: Linear graph (0-1-2-3-4)" << std::endl;
    Graph graph1(5);
    graph1.addEdge(0, 1, 1.0);
    graph1.addEdge(1, 2, 1.0);
    graph1.addEdge(2, 3, 1.0);
    graph1.addEdge(3, 4, 1.0);
    
    std::cout << "Graph k parameter: " << graph1.getK() << std::endl;
    
    // Initial frontier set S = {0}
    std::unordered_set<int> S1 = {0};
    
    // Initial distances (assuming we ran Dijkstra from source 0)
    std::vector<double> d_hat1 = {0.0, 1.0, 2.0, 3.0, 4.0};
    
    double B1 = 10.0; // Upper bound
    
    std::cout << "\nInput parameters:" << std::endl;
    printSet(S1, "Frontier set S");
    printDistances(d_hat1, "Distance estimates d_hat");
    std::cout << "Upper bound B: " << B1 << std::endl;
    
    FindPivotResult result1 = findPivots(graph1, B1, S1, d_hat1);
    
    std::cout << "\nResults:" << std::endl;
    printSet(result1.pivots, "Pivots");
    printSet(result1.nearby, "Nearby vertices");
    
    // Test 2: Star graph
    std::cout << "\n\n" << "Test 2: Star graph (center=0, leaves=1,2,3,4)" << std::endl;
    Graph graph2(5);
    graph2.addEdge(0, 1, 1.0);
    graph2.addEdge(0, 2, 1.0);
    graph2.addEdge(0, 3, 1.0);
    graph2.addEdge(0, 4, 1.0);
    
    std::cout << "Graph k parameter: " << graph2.getK() << std::endl;
    
    std::unordered_set<int> S2 = {0};
    std::vector<double> d_hat2 = {0.0, 1.0, 1.0, 1.0, 1.0};
    double B2 = 5.0;
    
    std::cout << "\nInput parameters:" << std::endl;
    printSet(S2, "Frontier set S");
    printDistances(d_hat2, "Distance estimates d_hat");
    std::cout << "Upper bound B: " << B2 << std::endl;
    
    FindPivotResult result2 = findPivots(graph2, B2, S2, d_hat2);
    
    std::cout << "\nResults:" << std::endl;
    printSet(result2.pivots, "Pivots");
    printSet(result2.nearby, "Nearby vertices");
    
    // Test 3: More complex graph
    std::cout << "\n\n" << "Test 3: Complex graph" << std::endl;
    Graph graph3(6);
    graph3.addEdge(0, 1, 1.0);
    graph3.addEdge(0, 2, 2.0);
    graph3.addEdge(1, 3, 1.0);
    graph3.addEdge(2, 3, 1.0);
    graph3.addEdge(3, 4, 1.0);
    graph3.addEdge(3, 5, 2.0);
    
    std::cout << "Graph k parameter: " << graph3.getK() << std::endl;
    
    std::unordered_set<int> S3 = {0};
    std::vector<double> d_hat3 = {0.0, 1.0, 2.0, 2.0, 3.0, 4.0};
    double B3 = 8.0;
    
    std::cout << "\nInput parameters:" << std::endl;
    printSet(S3, "Frontier set S");
    printDistances(d_hat3, "Distance estimates d_hat");
    std::cout << "Upper bound B: " << B3 << std::endl;
    
    FindPivotResult result3 = findPivots(graph3, B3, S3, d_hat3);
    
    std::cout << "\nResults:" << std::endl;
    printSet(result3.pivots, "Pivots");
    printSet(result3.nearby, "Nearby vertices");
    
    // Test 4: Test with multiple frontier vertices
    std::cout << "\n\n" << "Test 4: Multiple frontier vertices" << std::endl;
    std::unordered_set<int> S4 = {0, 1};
    std::vector<double> d_hat4 = {0.0, 0.0, 1.0, 1.0, 2.0, 2.0};
    double B4 = 6.0;
    
    std::cout << "\nInput parameters:" << std::endl;
    printSet(S4, "Frontier set S");
    printDistances(d_hat4, "Distance estimates d_hat");
    std::cout << "Upper bound B: " << B4 << std::endl;
    
    FindPivotResult result4 = findPivots(graph3, B4, S4, d_hat4);
    
    std::cout << "\nResults:" << std::endl;
    printSet(result4.pivots, "Pivots");
    printSet(result4.nearby, "Nearby vertices");
    
    return 0;
}
