#include "FindPivot.h"
#include "Graph.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>

FindPivotResult findPivots(Graph& graph, 
    double B,  //upper bound
    std::unordered_set<int>& S, // frontier set
    std::vector<double>& d_hat) {//current best distance
        int k = graph.getK();
        FindPivotResult results;

        std::unordered_set<int> W = S;
        std::vector<std::unordered_set<int>> W_steps(k + 1);
        W_steps[0] = S;

        for (int idx = 1; idx <= k; idx++) {
            W_steps[idx] = {};
            for (int u : W_steps[idx - 1]) {
                for (const auto& e : graph.getConnections(u)) {
                    if (d_hat[u] + e.weight <= d_hat[e.dest]) {
                        d_hat[e.dest] = d_hat[u] + e.weight;
                        if (d_hat[e.dest] < B) {
                            W_steps[idx].insert(e.dest);
                        }
                    }
                }
            }
            W.insert(W_steps[idx].begin(), W_steps[idx].end());
            if (W.size() > k * S.size()) {
                results.pivots = S;
                results.nearby = W;
                return results;
            }
        }

        // final 
        int numVertices = graph.getNumVertices();
        std::vector<int> predecessors(numVertices, -1);
        for (int u : W) {
            for (const auto& e : graph.getConnections(u)){
                if (W.count(u) && W.count(e.dest) && d_hat[e.dest] == d_hat[u] + e.weight){
                    predecessors[e.dest] = u;
                }
            }
        }


        std::unordered_map<int, int> tree_sizes;

        // Loop through every vertex 'v' in our forest...
        for (int v : W) {
            // Find the root of the tree containing 'v'
            int current_node = v;
            while (predecessors[current_node] != -1) {
                current_node = predecessors[current_node];
            }
            int root = current_node;

            tree_sizes[root]++;
        }
        std::unordered_set<int> P;
        for (const auto& pair: tree_sizes) {
            if (pair.second >= k) {
                P.insert(pair.first);
            }
        }
        results.pivots = P;
        results.nearby = W;
        return results;
        // std::unordered_set
}; 
