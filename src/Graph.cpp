#include "Graph.h"
#include "Debug.h"
#include <vector>
#include <iostream>
#include <cmath>

Graph::Graph (int n) {
    DEBUG_FUNCTION_ENTRY("Graph::Graph", "n=" << n);

    this->num_vertices = n;
    this->adjList.resize(n);

    DEBUG_MEMORY("Allocated adjacency list with size=" << adjList.size());

    this->calcK();
    this->calcT();

    DEBUG_PRINT("Graph created: n=" << num_vertices << ", k=" << k << ", t=" << t);
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

// Copy constructor
Graph::Graph(const Graph& other)
    : num_vertices(other.num_vertices), adjList(other.adjList), k(other.k), t(other.t) {
}

// Assignment operator
Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        num_vertices = other.num_vertices;
        adjList = other.adjList;
        k = other.k;
        t = other.t;
    }
    return *this;
}

void Graph::addEdge(int src, int dest, double weight) {
    DEBUG_FUNCTION_ENTRY("Graph::addEdge", "src=" << src << ", dest=" << dest << ", weight=" << weight);

    DEBUG_BOUNDS_CHECK(src, num_vertices, "source vertex");
    DEBUG_BOUNDS_CHECK(dest, num_vertices, "destination vertex");

    Edge e;
    e.dest = dest;
    e.weight = weight;

    size_t old_size = this->adjList[src].size();
    this->adjList[src].push_back(e);

    DEBUG_DATASTRUCTURE("ADD_EDGE", "adjList[" << src << "] size: " << old_size << " -> " << adjList[src].size());
}

std::vector<Edge> Graph::getConnections(int src) const {
    DEBUG_BOUNDS_CHECK(src, num_vertices, "source vertex in getConnections");

    if (src >= 0 && src < num_vertices) {
        return this->adjList[src];
    } else {
        DEBUG_PRINT("WARNING: Invalid vertex " << src << " in getConnections, returning empty vector");
        return std::vector<Edge>();
    }
}

int Graph::getNumVertices() const {
    return this->num_vertices;
}

void Graph::calcK(){
    double n = (double)this->num_vertices;
    this->k = std::floor(std::cbrt(std::log(n)));
}
int Graph::getK() const {
    return this->k;
}

void Graph::calcT(){
    double n = (double)this->num_vertices;
    double tmp = std::cbrt(std::log(n));
    this->t = std::floor(tmp * tmp);
}
int Graph::getT() const {
    return this->t;
}

void Graph::printAdjacencyList() {
    int idx = 0;
    for (const auto& edges : this->adjList){
        for (const auto& e : edges){
            std::cout << "(" << idx << ", " << e.dest << ", " << e.weight << ")" << std::endl;
        }
        idx++;
    }
}