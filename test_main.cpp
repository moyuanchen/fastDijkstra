#include <iostream>
#include <vector>
#include "Graph.h"
#include "Dijkstra.h"

int main() {
    std::cout << "=== Testing Graph and Dijkstra Implementation ===\n" << std::endl;
    
    // Test 1: Create a simple graph with 5 vertices
    std::cout << "Test 1: Creating a simple graph with 5 vertices" << std::endl;
    Graph graph(5);
    
    // Add edges manually
    graph.addEdge(0, 1, 4.0);
    graph.addEdge(0, 2, 2.0);
    graph.addEdge(1, 2, 1.0);
    graph.addEdge(1, 3, 5.0);
    graph.addEdge(2, 3, 8.0);
    graph.addEdge(2, 4, 10.0);
    graph.addEdge(3, 4, 2.0);
    
    std::cout << "\nAdjacency List:" << std::endl;
    graph.printAdjacencyList();
    
    // Test 2: Run Dijkstra from source vertex 0
    std::cout << "\n\nTest 2: Running Dijkstra from source vertex 0" << std::endl;
    DijkstraResults result = runDijkstra(graph, 0);
    
    std::cout << "\nShortest distances from vertex 0:" << std::endl;
    for (int i = 0; i < graph.getNumVertices(); i++) {
        std::cout << "Distance to vertex " << i << ": " << result.distances[i] << std::endl;
    }
    
    std::cout << "\nPredecessors:" << std::endl;
    for (int i = 0; i < graph.getNumVertices(); i++) {
        std::cout << "Predecessor of vertex " << i << ": " << result.predecessors[i] << std::endl;
    }
    
    // Test 3: Create a graph using the constructor with edges and weights
    std::cout << "\n\nTest 3: Creating graph using constructor with edges and weights" << std::endl;
    std::vector<std::vector<int>> edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}};
    std::vector<double> weights = {1.0, 4.0, 2.0, 3.0, 1.0};
    
    Graph graph2(5, edges, weights);
    std::cout << "\nGraph 2 Adjacency List:" << std::endl;
    graph2.printAdjacencyList();
    
    std::cout << "\nRunning Dijkstra on Graph 2 from vertex 0:" << std::endl;
    DijkstraResults result2 = runDijkstra(graph2, 0);
    
    std::cout << "\nShortest distances from vertex 0:" << std::endl;
    for (int i = 0; i < graph2.getNumVertices(); i++) {
        std::cout << "Distance to vertex " << i << ": " << result2.distances[i] << std::endl;
    }
    
    return 0;
}
