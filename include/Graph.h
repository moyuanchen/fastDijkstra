#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
// #include <iostream>


struct Edge {
    int dest;
    double weight = 1.0;
};

class Graph {
    private:
    int num_vertices;
    // const float default_weight = 1.0;
    std::vector<std::vector<Edge>> adjList;
    int k; int t;// parameters

    public:
    Graph(int n);
    Graph(int n, const std::vector<std::vector<int>>& edges);
    Graph(int n, const std::vector<std::vector<int>>& edges, const std::vector<double>& weights);

    // Copy constructor
    Graph(const Graph& other);

    // Assignment operator
    Graph& operator=(const Graph& other);

    int getNumVertices() const;
    std::vector<Edge> getConnections(int src) const;
    // void buildGraph(const std::vector<std::vector<Edge>>& edges, const std::vector<double>& weights);
    void addEdge(int src, int dest, double weight = 1.0);

    void calcK();
    void calcT();
    int getT() const;
    int getK() const;

    void printAdjacencyList();

};

#endif // GRAPH_H