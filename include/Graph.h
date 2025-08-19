#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>


struct Edge {
    int dest;
    double weight = 1.0;
};

class Graph {
    private:
    int num_vertices;
    // const float default_weight = 1.0;
    std::vector<std::vector<Edge>> adjList;

    public:
    Graph(int n);
    Graph(int n, const std::vector<std::vector<int>>& edges);
    Graph(int n, const std::vector<std::vector<int>>& edges, const std::vector<double>& weights);

    float getEdgeWeight(int u, int v);
    // void buildGraph(const std::vector<std::vector<Edge>>& edges, const std::vector<double>& weights);
    void addEdge(int src, int dest, double weight);
    void printAdjacencyMatrix();

};

#endif // GRAPH_H