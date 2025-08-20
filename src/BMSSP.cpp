#include "BMSSP.h"
#include "Graph.h"
#include "FindPivot.h"
#include "BatchHeap.h"
#include "Debug.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <limits>
#include <algorithm>
#include <iostream>

BaseCaseResults runBaseCase(Graph& graph, int src, double B) {
    DEBUG_FUNCTION_ENTRY("runBaseCase", "src=" << src << ", B=" << B);
    
    int numVertices = graph.getNumVertices();
    int k = graph.getK();
    
    DEBUG_PRINT("numVertices=" << numVertices << ", k=" << k);
    DEBUG_BOUNDS_CHECK(src, numVertices, "src");
    
    std::vector<double> distances(numVertices, std::numeric_limits<double>::max());
    std::vector<int> predecessors(numVertices , -1);
    std::unordered_set<int> U; 
    
    DEBUG_MEMORY("Allocated distances vector size=" << distances.size() << ", predecessors size=" << predecessors.size());
    
    // define pq
    using State = std::pair<double, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    distances[src] = 0.0;
    pq.push({0.0, src});

    int settled_nodes = 0;
    
    DEBUG_PRINT("Starting Dijkstra loop, target settled_nodes=" << (k + 1));
    
    // The loop now has two conditions, just as you suggested
    while (!pq.empty() && settled_nodes < k + 1) {
        DEBUG_LOOP(settled_nodes, "pq.size()=" << pq.size() << ", U.size()=" << U.size());
        
        double distance = pq.top().first;
        int vertex = pq.top().second;
        pq.pop();

        DEBUG_BOUNDS_CHECK(vertex, numVertices, "vertex from pq");
        DEBUG_PRINT("Processing vertex=" << vertex << ", distance=" << distance);

        // This check prevents us from processing an outdated, longer path to a vertex
        if (distance > distances[vertex]) {
            DEBUG_PRINT("Skipping outdated path to vertex=" << vertex);
            continue;
        }

        // We have now settled this vertex
        settled_nodes++;
        U.insert(vertex);
        
        DEBUG_PRINT("Settled vertex=" << vertex << ", settled_nodes=" << settled_nodes);

        // Relax neighbors
        for (const auto& edge : graph.getConnections(vertex)) {
            double altWeight = edge.weight + distance;
            int neighbor = edge.dest;
            
            DEBUG_BOUNDS_CHECK(neighbor, numVertices, "neighbor");
            DEBUG_PRINT("Relaxing edge " << vertex << "->" << neighbor << ", weight=" << edge.weight << ", altWeight=" << altWeight);

            if (altWeight <= distances[neighbor] && altWeight < B) {
                distances[neighbor] = altWeight;
                predecessors[neighbor] = vertex;
                distances[neighbor] = altWeight;
                predecessors[neighbor] = vertex;
                pq.push({altWeight, neighbor});
                DEBUG_PRINT("Updated distance[" << neighbor << "]=" << altWeight);
            }
        }
    }
    
    DEBUG_PRINT("Dijkstra loop completed. U.size()=" << U.size() << ", settled_nodes=" << settled_nodes);
    
    // Prepare results
    BaseCaseResults results;
    
    if (U.size() <= k) {
        DEBUG_PRINT("U.size() <= k, returning original bound B=" << B);
        results.B = B;
        results.U = U;
    } else {
        DEBUG_PRINT("U.size() > k, computing new bound from " << U.size() << " vertices");
        
        // According to the paper, we need to find the k-th smallest distance
        // among the settled vertices to get the new bound B'
        std::vector<double> settled_distances;
        settled_distances.reserve(U.size());
        
        for (int v : U) {
            DEBUG_BOUNDS_CHECK(v, numVertices, "vertex in U");
            settled_distances.push_back(distances[v]);
        }
        
        DEBUG_MEMORY("settled_distances vector size=" << settled_distances.size());
        
        // Sort distances to find the k-th smallest (0-indexed, so k-1)
        std::sort(settled_distances.begin(), settled_distances.end());
        
        if (k > 0 && k <= static_cast<int>(settled_distances.size())) {
            double B_prime = settled_distances[k - 1]; // k-th smallest distance
            DEBUG_PRINT("Found B_prime=" << B_prime << " as " << k << "-th smallest distance");
            
            // Create new U with vertices having distance <= B_prime
            std::unordered_set<int> new_U;
            for (int v : U) {
                DEBUG_BOUNDS_CHECK(v, numVertices, "vertex in U for new_U");
                if (distances[v] <= B_prime) {
                    new_U.insert(v);
                }
            }

            DEBUG_PRINT("Created new_U with size=" << new_U.size());
            results.B = B_prime;
            results.U = new_U;
        } else {
            DEBUG_PRINT("ERROR: Invalid k=" << k << " for settled_distances.size()=" << settled_distances.size());
            results.B = B;
            results.U = U;
        }
    }
    
    DEBUG_FUNCTION_EXIT("runBaseCase", "B=" << results.B << ", U.size()=" << results.U.size());
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
    DEBUG_FUNCTION_ENTRY("runBMSSP", "level=" << level << ", B=" << B << ", S.size()=" << S.size() << ", S=" << vectorToString(S));
    
    int k = graph.getK();
    int t = graph.getT();
    int numVertices = graph.getNumVertices();
    
    DEBUG_PRINT("Graph parameters: k=" << k << ", t=" << t << ", numVertices=" << numVertices);
    DEBUG_POINTER_CHECK(&distances, "distances");
    DEBUG_POINTER_CHECK(&predecessors, "predecessors");
    
    if (distances.size() != static_cast<size_t>(numVertices)) {
        DEBUG_PRINT("ERROR: distances.size()=" << distances.size() << " != numVertices=" << numVertices);
    }
    if (predecessors.size() != static_cast<size_t>(numVertices)) {
        DEBUG_PRINT("ERROR: predecessors.size()=" << predecessors.size() << " != numVertices=" << numVertices);
    }
    
    // Validate source vertices
    for (int src : S) {
        DEBUG_BOUNDS_CHECK(src, numVertices, "source vertex");
    }
    
    // Base case (line 2-3)
    if (level == 0) {
        DEBUG_PRINT("Base case: level=0, running base case for each source");
        
        // Convert S to unordered_set for compatibility with existing base case
        std::unordered_set<int> S_set(S.begin(), S.end());
        
        // Run base case on each source separately and combine results
        BMSSPResult result;
        result.new_bound = B;
        result.completed_vertices.clear();
        
        std::unordered_set<int> completed_set; // Track unique completed vertices
        
        for (int src : S) {
            DEBUG_PRINT("Running base case for source=" << src);
            BaseCaseResults base_result = runBaseCase(graph, src, B);
            
            DEBUG_PRINT("Base case result: B=" << base_result.B << ", U.size()=" << base_result.U.size());
            
            // Update bound to minimum encountered
            if (base_result.B < result.new_bound) {
                DEBUG_PRINT("Updating bound from " << result.new_bound << " to " << base_result.B);
                result.new_bound = base_result.B;
            }
            
            // Add vertices from this base case to completed vertices (avoid duplicates)
            for (int v : base_result.U) {
                DEBUG_BOUNDS_CHECK(v, numVertices, "vertex from base case U");
                if (completed_set.find(v) == completed_set.end()) {
                    completed_set.insert(v);
                    result.completed_vertices.push_back(v);
                }
            }
        }
        
        DEBUG_FUNCTION_EXIT("runBMSSP [base case]", "B=" << result.new_bound << ", completed.size()=" << result.completed_vertices.size());
        return result;
    }
    
    DEBUG_PRINT("Recursive case: level=" << level);
    
    // Find pivots (line 4)
    std::unordered_set<int> S_set(S.begin(), S.end());
    DEBUG_PRINT("Calling findPivots with B=" << B << ", S_set.size()=" << S_set.size());
    
    FindPivotResult pivot_result = findPivots(graph, B, S_set, distances);
    std::unordered_set<int> P = pivot_result.pivots;
    std::unordered_set<int> W = pivot_result.nearby;
    
    DEBUG_PRINT("findPivots result: P.size()=" << P.size() << ", W.size()=" << W.size());
    DEBUG_PRINT("P=" << setToString(P));
    DEBUG_PRINT("W=" << setToString(W));
    
    // Initialize BatchHeap D (line 5)
    int M = 1 << ((level - 1) * t); // 2^((l-1)*t)
    
    DEBUG_PRINT("Initializing BatchHeap with M=" << M << ", B_int=" << static_cast<int>(B));
    DEBUG_MEMORY("Creating BatchHeap D with M=" << M);
    
    BatchHeap D(M, static_cast<int>(B));
    
    // Insert pivots into D (line 6)
    DEBUG_PRINT("Inserting " << P.size() << " pivots into BatchHeap");
    for (int x : P) {
        DEBUG_BOUNDS_CHECK(x, numVertices, "pivot vertex");
        DEBUG_DATASTRUCTURE("INSERT", "vertex=" << x << ", distance=" << distances[x]);
        D.insert(x, distances[x]);
    }
    
    // Initialize variables (line 7)
    int i = 0;
    double B_prime_0 = B; // Default if P is empty
    if (!P.empty()) {
        B_prime_0 = std::numeric_limits<double>::max();
        for (int x : P) {
            DEBUG_BOUNDS_CHECK(x, numVertices, "pivot vertex for B_prime_0");
            B_prime_0 = std::min(B_prime_0, distances[x]);
        }
    }
    
    DEBUG_PRINT("Initialized B_prime_0=" << B_prime_0);
    
    std::vector<int> U; // Will accumulate completed vertices
    DEBUG_MEMORY("Initialized U vector");
    
    // Main loop (line 8)
    int target_size = k * (1 << (level * t)); // k * 2^(l*t)
    
    // Logical constraint: can't have more vertices than exist in graph
    if (target_size > graph.getNumVertices()) {
        DEBUG_PRINT("Capping target_size from " << target_size << " to " << graph.getNumVertices());
        target_size = graph.getNumVertices();
    }
    
    DEBUG_PRINT("Starting main loop with target_size=" << target_size);
    
    while (static_cast<int>(U.size()) < target_size) {
        DEBUG_LOOP(i, "U.size()=" << U.size() << ", target_size=" << target_size);
        
        // Check if D is empty
        PullResults pull_result;
        try {
            DEBUG_DATASTRUCTURE("PULL", "attempting to pull from BatchHeap");
            pull_result = D.pull();
            if (pull_result.vertices.empty()) {
                DEBUG_PRINT("D.pull() returned empty vertices, breaking loop");
                break; // D is empty
            }
            DEBUG_DATASTRUCTURE("PULL", "pulled " << pull_result.vertices.size() << " vertices, new_bound=" << pull_result.new_bound);
        } catch (const std::exception& e) {
            DEBUG_PRINT("Exception in D.pull(): " << e.what());
            break; // D is empty or error occurred
        } catch (...) {
            DEBUG_PRINT("Unknown exception in D.pull()");
            break; // D is empty or error occurred
        }
        
        i++;
        double B_i = pull_result.new_bound;
        std::vector<int> S_i = pull_result.vertices;
        
        DEBUG_PRINT("Iteration " << i << ": B_i=" << B_i << ", S_i.size()=" << S_i.size());
        DEBUG_PRINT("S_i=" << vectorToString(S_i));
        
        // Validate S_i vertices
        for (int v : S_i) {
            DEBUG_BOUNDS_CHECK(v, numVertices, "vertex in S_i");
        }
        
        // Recursive call (line 11)
        DEBUG_PRINT("Making recursive call with level=" << (level-1) << ", B_i=" << B_i);
        BMSSPResult recursive_result = runBMSSP(graph, distances, predecessors, 
                                               level - 1, B_i, S_i);
        double B_prime_i = recursive_result.new_bound;
        std::vector<int> U_i = recursive_result.completed_vertices;
        
        DEBUG_PRINT("Recursive result: B_prime_i=" << B_prime_i << ", U_i.size()=" << U_i.size());
        
        // Update U (line 12)
        size_t old_U_size = U.size();
        U.insert(U.end(), U_i.begin(), U_i.end());
        DEBUG_PRINT("Updated U from size " << old_U_size << " to " << U.size());
        
        // Edge relaxation and data structure updates (lines 13-21)
        std::list<std::pair<int, double>> K;
        DEBUG_PRINT("Starting edge relaxation for " << U_i.size() << " vertices");
        
        for (int u : U_i) {
            DEBUG_BOUNDS_CHECK(u, numVertices, "vertex u in U_i");
            
            for (const auto& edge : graph.getConnections(u)) {
                int v = edge.dest;
                double new_dist = distances[u] + edge.weight;
                
                DEBUG_BOUNDS_CHECK(v, numVertices, "edge destination");
                DEBUG_PRINT("Considering edge " << u << "->" << v << ", weight=" << edge.weight << ", new_dist=" << new_dist);
                
                // Relaxation (lines 15-16)
                if (new_dist < distances[v]) {
                    DEBUG_PRINT("Relaxing: distances[" << v << "] from " << distances[v] << " to " << new_dist);
                    distances[v] = new_dist;
                    predecessors[v] = u;
                    
                    // Check intervals for insertion (lines 17-20)
                    if (new_dist >= B_i && new_dist < B) {
                        // Insert into D (line 18)
                        DEBUG_DATASTRUCTURE("INSERT", "vertex=" << v << ", distance=" << new_dist << " [B_i, B)");
                        D.insert(v, new_dist);
                    } else if (new_dist >= B_prime_i && new_dist < B_i) {
                        // Add to K for batch prepend (line 20)
                        DEBUG_PRINT("Adding to K: vertex=" << v << ", distance=" << new_dist << " [B_prime_i, B_i)");
                        K.push_back({v, new_dist});
                    }
                }
            }
        }
        
        // Batch prepend (line 21)
        // Add vertices from S_i that are in [B'_i, B_i)
        DEBUG_PRINT("Adding vertices from S_i to K for [B_prime_i, B_i) interval");
        for (int x : S_i) {
            DEBUG_BOUNDS_CHECK(x, numVertices, "vertex x in S_i");
            if (distances[x] >= B_prime_i && distances[x] < B_i) {
                DEBUG_PRINT("Adding S_i vertex to K: vertex=" << x << ", distance=" << distances[x]);
                K.push_back({x, distances[x]});
            }
        }
        
        if (!K.empty()) {
            DEBUG_DATASTRUCTURE("BATCHPREPEND", "K.size()=" << K.size());
            D.batchPrepend(K);
        } else {
            DEBUG_PRINT("K is empty, skipping batchPrepend");
        }
    }
    
    DEBUG_PRINT("Main loop completed after " << i << " iterations, final U.size()=" << U.size());
    
    // Prepare final result (line 22)
    BMSSPResult result;
    
    // The new bound should be the minimum distance in the completed set
    // that ensures we have exactly the required number of vertices
    double final_bound = B;
    if (!U.empty()) {
        // Find the maximum distance among completed vertices from U
        double max_distance_in_U = 0.0;
        for (int v : U) {
            DEBUG_BOUNDS_CHECK(v, numVertices, "vertex v in final U");
            if (distances[v] > max_distance_in_U) {
                max_distance_in_U = distances[v];
            }
        }
        final_bound = std::min(final_bound, max_distance_in_U);
        DEBUG_PRINT("Computed max_distance_in_U=" << max_distance_in_U);
    }
    
    // Also consider B_prime_0 if it's valid
    if (B_prime_0 < B && !P.empty()) {
        final_bound = std::min(final_bound, B_prime_0);
        DEBUG_PRINT("Considering B_prime_0=" << B_prime_0);
    }
    
    DEBUG_PRINT("Final bound computed: " << final_bound);
    
    result.new_bound = final_bound;
    result.completed_vertices = U;
    
    // Add vertices from W with distance <= final_bound (avoid duplicates)
    std::unordered_set<int> completed_set(U.begin(), U.end());
    size_t vertices_before_W = result.completed_vertices.size();
    
    for (int x : W) {
        DEBUG_BOUNDS_CHECK(x, numVertices, "vertex x in W");
        if (distances[x] <= final_bound && completed_set.find(x) == completed_set.end()) {
            result.completed_vertices.push_back(x);
            completed_set.insert(x);
        }
    }
    
    DEBUG_PRINT("Added " << (result.completed_vertices.size() - vertices_before_W) << " vertices from W");
    
    DEBUG_FUNCTION_EXIT("runBMSSP", "B=" << result.new_bound << ", completed.size()=" << result.completed_vertices.size());
    return result;
}