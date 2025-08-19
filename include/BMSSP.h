#ifndef BMSSP_H
#define BMSSP_H
#include "Graph.h"
#include<unordered_set>

struct BaseCaseResults {
    int B;
    std::unordered_set<int> U;
};

BaseCaseResults runBaseCase(Graph& graph, int src, int B);

#endif