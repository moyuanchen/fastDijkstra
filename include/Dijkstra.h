#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include "Graph.h"

struct DijkstraResults {
    std::vector<int> predecessors;
    std::vector<double> distances;
};

DijkstraResults runDijkstra(Graph& graph, int source);

#endif