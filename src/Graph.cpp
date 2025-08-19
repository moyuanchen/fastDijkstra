#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

class Vertex {};

class Graph {
public:
    
    Graph(int n, const std::vector<std::tuple<int, int>>& edges)
        : num_vertices(n), adjacency_matrix(n, std::vector<double>(n, 0.0)) {
            // Initialize the adjacency matrix with default weights 1
            for (const auto& edge : edges) {
                int u = std::get<0>(edge);
                int v = std::get<1>(edge);
                adjacency_matrix[u][v] = 1.0;  // Default weight
                adjacency_matrix[v][u] = 1.0;  // Default weight
            }

    }

    Graph(int n, const std::vector<std::tuple<int, int>>& edges, const std::vector<double>& weights)
        : num_vertices(n), adjacency_matrix(n, std::vector<double>(n, 0.0)){
            // Initialize the adjacency matrix with given weights
            for (size_t i = 0; i < edges.size(); ++i) {
                int u = std::get<0>(edges[i]);
                int v = std::get<1>(edges[i]);
                double weight = weights[i];
                adjacency_matrix[u][v] = weight;
                adjacency_matrix[v][u] = weight;
            }

    }

    float getEdgeWeight(int u, int v) const {
        return adjacency_matrix[u][v];
    }

    void printAdjacencyMatrix() const {
        for (int i = 0; i < num_vertices; ++i) {
            for (int j = 0; j < num_vertices; ++j) {
                std::cout << adjacency_matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    int num_vertices;
    std::vector<std::vector<double>> adjacency_matrix;
    // std::vector<double> edge_weights;
};

#endif // GRAPH_H