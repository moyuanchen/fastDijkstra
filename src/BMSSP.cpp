#include "BMSSP.h"
#include "Graph.h"
#include "FindPivot.h"
#include "BatchHeap.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <limits>
#include <algorithm>
#include <iostream>

BaseCaseResults runBaseCase(Graph& graph, int src, double B) {
    int numVertices = graph.getNumVertices();
    int k = graph.getK();
    std::vector<double> distances(numVertices, std::numeric_limits<double>::max());
    std::vector<int> predecessors(numVertices , -1);
    std::unordered_set<int> U; 
    
    // define pq
    using State = std::pair<double, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    distances[src] = 0.0;
    pq.push({0.0, src});

    int settled_nodes = 0;
    
    // The loop now has two conditions, just as you suggested
    while (!pq.empty() && settled_nodes < k + 1) {
        double distance = pq.top().first;
        int vertex = pq.top().second;
        pq.pop();

        // This check prevents us from processing an outdated, longer path to a vertex
        if (distance > distances[vertex]) {
            continue;
        }

        // We have now settled this vertex
        settled_nodes++;
        U.insert(vertex);

        // Relax neighbors
        for (const auto& edge : graph.getConnections(vertex)) {
            double altWeight = edge.weight + distance;
            int neighbor = edge.dest;

            if (altWeight <= distances[neighbor] && altWeight < B) {
                distances[neighbor] = altWeight;
                predecessors[neighbor] = vertex;
                pq.push({altWeight, neighbor});
            }
        }
    }
    
    // Prepare results
    BaseCaseResults results;
    
    if (U.size() <= k) {
        results.B = B;
        results.U = U;
    } else {
        // According to the paper, we need to find the k-th smallest distance
        // among the settled vertices to get the new bound B'
        std::vector<double> settled_distances;
        for (int v : U) {
            settled_distances.push_back(distances[v]);
        }
        
        // Sort distances to find the k-th smallest (0-indexed, so k-1)
        std::sort(settled_distances.begin(), settled_distances.end());
        double B_prime = settled_distances[k - 1]; // k-th smallest distance
        
        // Create new U with vertices having distance <= B_prime
        std::unordered_set<int> new_U;
        for (int v : U) {
            if (distances[v] <= B_prime) {
                new_U.insert(v);
            }
        }

        results.B = B_prime;
        results.U = new_U;
    }
    
    return results;
}

BMSSPResult runBMSSP(
    Graph& graph,
    std::vector<double>& distances,
    std::vector<int>& predecessors,
    int level,
    double B,
    const std::vector<int>& S
) {
    int k = graph.getK();
    int t = graph.getT();
    
    // Base case (line 2-3)
    if (level == 0) {
        // Convert S to unordered_set for compatibility with existing base case
        std::unordered_set<int> S_set(S.begin(), S.end());
        
        // Run base case on each source separately and combine results
        BMSSPResult result;
        result.new_bound = B;
        result.completed_vertices.clear();
        
        std::unordered_set<int> completed_set; // Track unique completed vertices
        
        for (int src : S) {
            BaseCaseResults base_result = runBaseCase(graph, src, B);
            
            // Update bound to minimum encountered
            if (base_result.B < result.new_bound) {
                result.new_bound = base_result.B;
            }
            
            // Add vertices from this base case to completed vertices (avoid duplicates)
            for (int v : base_result.U) {
                if (completed_set.find(v) == completed_set.end()) {
                    completed_set.insert(v);
                    result.completed_vertices.push_back(v);
                }
            }
        }
        
        return result;
    }
    
    // Find pivots (line 4)
    std::unordered_set<int> S_set(S.begin(), S.end());
    FindPivotResult pivot_result = findPivots(graph, B, S_set, distances);
    std::unordered_set<int> P = pivot_result.pivots;
    std::unordered_set<int> W = pivot_result.nearby;
    
    // Initialize BatchHeap D (line 5)
    int M = 1 << ((level - 1) * t); // 2^((l-1)*t)
    BatchHeap D(M, static_cast<int>(B));
    
    // Insert pivots into D (line 6)
    for (int x : P) {
        D.insert(x, distances[x]);
    }
    
    // Initialize variables (line 7)
    int i = 0;
    double B_prime_0 = B; // Default if P is empty
    if (!P.empty()) {
        B_prime_0 = std::numeric_limits<double>::max();
        for (int x : P) {
            B_prime_0 = std::min(B_prime_0, distances[x]);
        }
    }
    
    std::vector<int> U; // Will accumulate completed vertices
    
    // Main loop (line 8)
    int target_size = k * (1 << (level * t)); // k * 2^(l*t)
    
    int loop_count = 0;
    while (static_cast<int>(U.size()) < target_size) {
        loop_count++;
        
        if (loop_count > 100) {  // Safety break to prevent infinite loops
            break;
        }
        
        // Check if D is empty
        PullResults pull_result;
        try {
            pull_result = D.pull();
            if (pull_result.vertices.empty()) {
                break; // D is empty
            }
        } catch (...) {
            break; // D is empty or error occurred
        }
        
        i++;
        double B_i = pull_result.new_bound;
        std::vector<int> S_i = pull_result.vertices;
        
        // Recursive call (line 11)
        BMSSPResult recursive_result = runBMSSP(graph, distances, predecessors, 
                                               level - 1, B_i, S_i);
        double B_prime_i = recursive_result.new_bound;
        std::vector<int> U_i = recursive_result.completed_vertices;
        
        // Update U (line 12)
        U.insert(U.end(), U_i.begin(), U_i.end());
        
        // Edge relaxation and data structure updates (lines 13-21)
        std::list<std::pair<int, double>> K;
        
        for (int u : U_i) {
            for (const auto& edge : graph.getConnections(u)) {
                int v = edge.dest;
                double new_dist = distances[u] + edge.weight;
                
                // Relaxation (lines 15-16)
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    predecessors[v] = u;
                    
                    // Check intervals for insertion (lines 17-20)
                    if (new_dist >= B_i && new_dist < B) {
                        // Insert into D (line 18)
                        D.insert(v, new_dist);
                    } else if (new_dist >= B_prime_i && new_dist < B_i) {
                        // Add to K for batch prepend (line 20)
                        K.push_back({v, new_dist});
                    }
                }
            }
        }
        
        // Batch prepend (line 21)
        // Add vertices from S_i that are in [B'_i, B_i)
        for (int x : S_i) {
            if (distances[x] >= B_prime_i && distances[x] < B_i) {
                K.push_back({x, distances[x]});
            }
        }
        
        if (!K.empty()) {
            D.batchPrepend(K);
        }
    }
    
    // Prepare final result (line 22)
    BMSSPResult result;
    
    // The new bound should be the minimum distance in the completed set
    // that ensures we have exactly the required number of vertices
    double final_bound = B;
    if (!U.empty()) {
        // Find the maximum distance among completed vertices from U
        double max_distance_in_U = 0.0;
        for (int v : U) {
            if (distances[v] > max_distance_in_U) {
                max_distance_in_U = distances[v];
            }
        }
        final_bound = std::min(final_bound, max_distance_in_U);
    }
    
    // Also consider B_prime_0 if it's valid
    if (B_prime_0 < B && !P.empty()) {
        final_bound = std::min(final_bound, B_prime_0);
    }
    
    result.new_bound = final_bound;
    result.completed_vertices = U;
    
    // Add vertices from W with distance <= final_bound (avoid duplicates)
    std::unordered_set<int> completed_set(U.begin(), U.end());
    for (int x : W) {
        if (distances[x] <= final_bound && completed_set.find(x) == completed_set.end()) {
            result.completed_vertices.push_back(x);
            completed_set.insert(x);
        }
    }
    
    return result;
}