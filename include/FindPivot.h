#ifndef FINDPIVOT_H
#define FINDPIVOT_H
#include "Graph.h"
#include <unordered_set>
#include <vector>

struct FindPivotResult {
    std::unordered_set<int> pivots;
    std::unordered_set<int> nearby;
};

FindPivotResult findPivots(
    Graph& graph, 
    double B,  //upper bound
    std::unordered_set<int>& S, // frontier set
    std::vector<double>& d_hat //current best distances
);


#endif