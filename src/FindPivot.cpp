#include "FindPivot.h"
#include "Graph.h"
#include "Debug.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>

FindPivotResult findPivots(Graph& graph,
    double B,  //upper bound
    std::unordered_set<int>& S, // frontier set
    std::vector<double>& d_hat) {//current best distance

    DEBUG_FUNCTION_ENTRY("findPivots", "B=" << B << ", S.size()=" << S.size() << ", S=" << setToString(S));

    int k = graph.getK();
    int numVertices = graph.getNumVertices();
    FindPivotResult results;

    DEBUG_PRINT("Graph parameters: k=" << k << ", numVertices=" << numVertices);
    DEBUG_POINTER_CHECK(&d_hat, "d_hat");

    if (d_hat.size() != static_cast<size_t>(numVertices)) {
        DEBUG_PRINT("ERROR: d_hat.size()=" << d_hat.size() << " != numVertices=" << numVertices);
    }

    // Validate vertices in S
    for (int v : S) {
        DEBUG_BOUNDS_CHECK(v, numVertices, "vertex in S");
    }

    std::unordered_set<int> W = S;
    std::vector<std::unordered_set<int>> W_steps(k + 1);
    std::vector<int> predecessors(numVertices, -1);
    std::unordered_map<int, int> tree_sizes;

    DEBUG_MEMORY("Allocated W_steps with size=" << W_steps.size() << ", predecessors size=" << predecessors.size());

    W_steps[0] = S;
    DEBUG_PRINT("Initialized W_steps[0] with S, size=" << W_steps[0].size());

    // Bellman-Ford Relaxation
    DEBUG_PRINT("Starting Bellman-Ford relaxation for " << k << " steps");
    for (int idx = 1; idx <= k; idx++) {
        DEBUG_LOOP(idx, "W_steps[" << (idx-1) << "].size()=" << W_steps[idx-1].size() << ", W.size()=" << W.size());

        W_steps[idx] = {};
        for (int u : W_steps[idx - 1]) {
            DEBUG_BOUNDS_CHECK(u, numVertices, "vertex u in W_steps");

            for (const auto& e : graph.getConnections(u)) {
                int dest = e.dest;
                double new_dist = d_hat[u] + e.weight;

                DEBUG_BOUNDS_CHECK(dest, numVertices, "edge destination");
                DEBUG_PRINT("Considering edge " << u << "->" << dest << ", weight=" << e.weight << ", new_dist=" << new_dist << ", current_dist=" << d_hat[dest]);

                if (new_dist <= d_hat[dest]) {
                    DEBUG_PRINT("Relaxing: d_hat[" << dest << "] from " << d_hat[dest] << " to " << new_dist);
                    d_hat[dest] = new_dist;
                    predecessors[dest] = u;
                    if (new_dist < B) {
                        DEBUG_PRINT("Adding vertex " << dest << " to W_steps[" << idx << "] (dist=" << new_dist << " < B=" << B << ")");
                        W_steps[idx].insert(dest);
                    }
                }
            }
        }
        W.insert(W_steps[idx].begin(), W_steps[idx].end());
        DEBUG_PRINT("After step " << idx << ": W_steps[" << idx << "].size()=" << W_steps[idx].size() << ", W.size()=" << W.size());

        if (W.size() > k * S.size()) {
            DEBUG_PRINT("Early termination: W.size()=" << W.size() << " > k*S.size()=" << (k * S.size()));
            results.pivots = S;
            results.nearby = W;
            DEBUG_FUNCTION_EXIT("findPivots [early]", "pivots.size()=" << results.pivots.size() << ", nearby.size()=" << results.nearby.size());
            return results;
        }
    }

    DEBUG_PRINT("Bellman-Ford relaxation completed, analyzing forest structure");

    // Loop through every vertex 'v' in our forest...
    for (int v : W) {
        DEBUG_BOUNDS_CHECK(v, numVertices, "vertex v in W");

        // Find the root of the tree containing 'v'
        int current_node = v;
        while (predecessors[current_node] != -1) {
            DEBUG_BOUNDS_CHECK(current_node, numVertices, "current_node in predecessor trace");
            current_node = predecessors[current_node];
        }
        int root = current_node;

        DEBUG_BOUNDS_CHECK(root, numVertices, "root vertex");
        tree_sizes[root]++;
    }

    DEBUG_PRINT("Computed tree sizes for " << tree_sizes.size() << " roots");

    std::unordered_set<int> P;

    DEBUG_PRINT("Selecting pivots from trees with size >= k=" << k);
    for (const auto& pair: tree_sizes) {
        DEBUG_BOUNDS_CHECK(pair.first, numVertices, "root vertex in tree_sizes");
        DEBUG_PRINT("Tree rooted at " << pair.first << " has size " << pair.second);

        if (pair.second >= k) {
            DEBUG_PRINT("Adding pivot: " << pair.first << " (tree size=" << pair.second << " >= k=" << k << ")");
            P.insert(pair.first);
        }
    }

    DEBUG_PRINT("Selected " << P.size() << " pivots: " << setToString(P));

    results.pivots = P;
    results.nearby = W;

    DEBUG_FUNCTION_EXIT("findPivots", "pivots.size()=" << results.pivots.size() << ", nearby.size()=" << results.nearby.size());
    return results;
        // std::unordered_set
};
