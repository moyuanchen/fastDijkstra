#include "Graph.h"
#include <vector>
#include <iostream>
#include <cmath>

Graph::Graph (int n) {
    this->num_vertices = n;
    this->adjList.resize(n);
    this->calcK();
    this->calcT();
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

void Graph::addEdge(int src, int dest, double weight) {
    Edge e; e.dest = dest; e.weight = weight;
    this->adjList[src].push_back(e);
}

std::vector<Edge> Graph::getConnections(int src) {
    return this->adjList[src];
}

int Graph::getNumVertices(){
    return this->num_vertices;
}

void Graph::calcK(){
    double n = (double)this->num_vertices;
    this->k = std::floor(std::cbrt(std::log(n)));
}
int Graph::getK(){
    return this->k;
}

void Graph::calcT(){
    double n = (double)this->num_vertices;
    double tmp = std::cbrt(std::log(n));
    this->t = std::floor(tmp * tmp);
}
int Graph::getT(){
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