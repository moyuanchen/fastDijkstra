#include "Graph.h"

Graph::Graph (int n) {
    this->num_vertices = n;
    this->adjList.resize(n);
}

Graph::Graph (int n, const std::vector<std::vector<int>>& edges) : Graph(n) {
    for (const auto& edge : edges) {
        int src = edge[0]; int dest = edge[1];
        this->addEdge(src, dest);
    }
}

Graph::Graph (int n, const std::vector<std::vector<int>>& edges, const std::vector<double>& weights) : Graph(n) {
    int idx = 0;
    for (const auto& edge : edges) {
        int src = edge[0]; int dest = edge[1]; double weight = weights[idx++];
        this->addEdge(src, dest, weight);
    }
}

void Graph::addEdge(int src, int dest, double weight = 1.0) {
    Edge e; e.dest = dest; e.weight = weight;
    this->adjList[src].push_back(e);
}