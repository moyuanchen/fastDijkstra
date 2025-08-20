#ifndef BMSSP_H
#define BMSSP_H
#include "Graph.h"
#include<unordered_set>
#include<vector>

struct BaseCaseResults {
    double B;
    std::unordered_set<int> U;
};

BaseCaseResults runBaseCase(Graph& graph, int src, double B);

// A struct to hold the results of a BMSSP call, as described in the paper [cite: 109]
struct BMSSPResult {
    double new_bound;
    std::vector<int> completed_vertices;
};

// The declaration for the main recursive function
BMSSPResult runBMSSP(
    // --- Main data that doesn't change ---
    Graph& graph,
    std::vector<double>& distances,
    std::vector<int>& predecessors,

    // --- Parameters for this specific recursive call ---
    int level,                      // The current recursion level, l [cite: 106]
    double B,                       // The upper bound for this search [cite: 106]
    const std::vector<int>& S       // The set of source vertices for this sub-problem [cite: 106]
);


#endif