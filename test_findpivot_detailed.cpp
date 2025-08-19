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
    std::cout << "=== Detailed FindPivot Analysis ===\n" << std::endl;
    
    // Create a larger graph to see more interesting behavior
    std::cout << "Test: Larger graph (10 vertices)" << std::endl;
    Graph graph(10);
    
    // Create a more complex structure
    graph.addEdge(0, 1, 1.0);
    graph.addEdge(0, 2, 1.0);
    graph.addEdge(1, 3, 1.0);
    graph.addEdge(1, 4, 1.0);
    graph.addEdge(2, 5, 1.0);
    graph.addEdge(2, 6, 1.0);
    graph.addEdge(3, 7, 1.0);
    graph.addEdge(4, 7, 1.0);
    graph.addEdge(5, 8, 1.0);
    graph.addEdge(6, 8, 1.0);
    graph.addEdge(7, 9, 1.0);
    graph.addEdge(8, 9, 1.0);
    
    std::cout << "Graph structure:" << std::endl;
    graph.printAdjacencyList();
    std::cout << "\nGraph k parameter: " << graph.getK() << std::endl;
    
    // Test with different scenarios
    std::cout << "\n--- Scenario 1: Single source frontier ---" << std::endl;
    std::unordered_set<int> S1 = {0};
    std::vector<double> d_hat1 = {0.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0, 3.0, 3.0, 4.0};
    double B1 = 6.0;
    
    printSet(S1, "Frontier set S");
    printDistances(d_hat1, "Distance estimates d_hat");
    std::cout << "Upper bound B: " << B1 << std::endl;
    
    FindPivotResult result1 = findPivots(graph, B1, S1, d_hat1);
    printSet(result1.pivots, "Pivots");
    printSet(result1.nearby, "Nearby vertices");
    
    std::cout << "\n--- Scenario 2: Multiple source frontier ---" << std::endl;
    std::unordered_set<int> S2 = {1, 2};
    std::vector<double> d_hat2 = {1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 3.0};
    double B2 = 5.0;
    
    printSet(S2, "Frontier set S");
    printDistances(d_hat2, "Distance estimates d_hat");
    std::cout << "Upper bound B: " << B2 << std::endl;
    
    FindPivotResult result2 = findPivots(graph, B2, S2, d_hat2);
    printSet(result2.pivots, "Pivots");
    printSet(result2.nearby, "Nearby vertices");
    
    std::cout << "\n--- Scenario 3: Low upper bound (restrictive) ---" << std::endl;
    std::unordered_set<int> S3 = {0};
    std::vector<double> d_hat3 = {0.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0, 3.0, 3.0, 4.0};
    double B3 = 2.5; // Very restrictive bound
    
    printSet(S3, "Frontier set S");
    printDistances(d_hat3, "Distance estimates d_hat");
    std::cout << "Upper bound B: " << B3 << std::endl;
    
    FindPivotResult result3 = findPivots(graph, B3, S3, d_hat3);
    printSet(result3.pivots, "Pivots");
    printSet(result3.nearby, "Nearby vertices");
    
    std::cout << "\n--- Scenario 4: High upper bound (permissive) ---" << std::endl;
    std::unordered_set<int> S4 = {0};
    std::vector<double> d_hat4 = {0.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0, 3.0, 3.0, 4.0};
    double B4 = 10.0; // Very permissive bound
    
    printSet(S4, "Frontier set S");
    printDistances(d_hat4, "Distance estimates d_hat");
    std::cout << "Upper bound B: " << B4 << std::endl;
    
    FindPivotResult result4 = findPivots(graph, B4, S4, d_hat4);
    printSet(result4.pivots, "Pivots");
    printSet(result4.nearby, "Nearby vertices");
    
    return 0;
}
