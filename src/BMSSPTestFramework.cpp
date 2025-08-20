#include "BMSSPTestFramework.h"
#include "Dijkstra.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <random>

BMSSPTestFramework::BMSSPTestFramework(unsigned int seed) : rng(seed) {}

// Graph generation methods
Graph BMSSPTestFramework::generateRandomGraph(int n, int m, WeightDistribution weight_dist) {
    Graph graph(n);
    
    // Generate m random edges
    std::uniform_int_distribution<int> vertex_dist(0, n - 1);
    auto weights = generateWeights(m, weight_dist);
    
    for (int i = 0; i < m; ++i) {
        int src = vertex_dist(rng);
        int dest = vertex_dist(rng);
        
        // Avoid self-loops in most cases
        if (src != dest) {
            graph.addEdge(src, dest, weights[i]);
        }
    }
    
    return graph;
}

Graph BMSSPTestFramework::generateTreeGraph(int n, WeightDistribution weight_dist) {
    Graph graph(n);
    
    if (n <= 1) return graph;
    
    auto weights = generateWeights(n - 1, weight_dist);
    
    // Generate random tree using Prim's algorithm
    std::vector<bool> in_tree(n, false);
    in_tree[0] = true;
    
    for (int i = 1; i < n; ++i) {
        // Choose random vertex already in tree
        std::uniform_int_distribution<int> tree_vertex_dist(0, i - 1);
        int tree_vertex = tree_vertex_dist(rng);
        
        // Find the tree_vertex-th vertex that's already in the tree
        int count = 0;
        for (int v = 0; v < n; ++v) {
            if (in_tree[v]) {
                if (count == tree_vertex) {
                    tree_vertex = v;
                    break;
                }
                count++;
            }
        }
        
        // Add edge from tree_vertex to vertex i
        graph.addEdge(tree_vertex, i, weights[i - 1]);
        in_tree[i] = true;
    }
    
    return graph;
}

Graph BMSSPTestFramework::generateCycleGraph(int n, WeightDistribution weight_dist) {
    Graph graph(n);
    
    if (n < 3) return graph; // Need at least 3 vertices for a cycle
    
    auto weights = generateWeights(n, weight_dist);
    
    // Create cycle 0 -> 1 -> 2 -> ... -> n-1 -> 0
    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n;
        graph.addEdge(i, next, weights[i]);
    }
    
    return graph;
}

Graph BMSSPTestFramework::generateGrid2D(int rows, int cols, WeightDistribution weight_dist) {
    int n = rows * cols;
    Graph graph(n);
    
    auto weight_gen = [this, weight_dist]() { return generateWeight(weight_dist); };
    
    // Add edges for grid connectivity
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int current = r * cols + c;
            
            // Right edge
            if (c + 1 < cols) {
                int right = r * cols + (c + 1);
                graph.addEdge(current, right, weight_gen());
            }
            
            // Down edge
            if (r + 1 < rows) {
                int down = (r + 1) * cols + c;
                graph.addEdge(current, down, weight_gen());
            }
        }
    }
    
    return graph;
}

Graph BMSSPTestFramework::generateStarGraph(int n, WeightDistribution weight_dist) {
    Graph graph(n);
    
    if (n <= 1) return graph;
    
    auto weights = generateWeights(n - 1, weight_dist);
    
    // Vertex 0 is the center, connect to all others
    for (int i = 1; i < n; ++i) {
        graph.addEdge(0, i, weights[i - 1]);
    }
    
    return graph;
}

Graph BMSSPTestFramework::generateCompleteGraph(int n, WeightDistribution weight_dist) {
    Graph graph(n);
    
    auto weights = generateWeights(n * (n - 1), weight_dist);
    int weight_idx = 0;
    
    // Add all possible edges
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                graph.addEdge(i, j, weights[weight_idx++]);
            }
        }
    }
    
    return graph;
}

// Weight generation methods
double BMSSPTestFramework::generateWeight(WeightDistribution dist) {
    switch (dist) {
        case WeightDistribution::UNIFORM: {
            std::uniform_real_distribution<double> uniform_dist(0.1, 10.0);
            return uniform_dist(rng);
        }
        case WeightDistribution::EXPONENTIAL: {
            std::exponential_distribution<double> exp_dist(1.0);
            return exp_dist(rng);
        }
        case WeightDistribution::INTEGER_SMALL: {
            std::uniform_int_distribution<int> int_dist(1, 10);
            return static_cast<double>(int_dist(rng));
        }
        case WeightDistribution::INTEGER_LARGE: {
            std::uniform_int_distribution<int> int_dist(1, 1000);
            return static_cast<double>(int_dist(rng));
        }
        case WeightDistribution::UNIT_WEIGHTS:
            return 1.0;
        case WeightDistribution::BINARY_WEIGHTS: {
            std::uniform_int_distribution<int> binary_dist(0, 1);
            return binary_dist(rng) ? 1.0 : 2.0;
        }
        case WeightDistribution::POWER_OF_TWO: {
            std::uniform_int_distribution<int> power_dist(0, 6);
            return static_cast<double>(1 << power_dist(rng));
        }
        default:
            return 1.0;
    }
}

std::vector<double> BMSSPTestFramework::generateWeights(int count, WeightDistribution dist) {
    std::vector<double> weights;
    weights.reserve(count);
    
    for (int i = 0; i < count; ++i) {
        weights.push_back(generateWeight(dist));
    }
    
    return weights;
}

// Source set generation methods
std::vector<int> BMSSPTestFramework::generateRandomSources(const Graph& g, int count) {
    int n = g.getNumVertices();
    count = std::min(count, n);
    
    std::vector<int> all_vertices(n);
    std::iota(all_vertices.begin(), all_vertices.end(), 0);
    
    std::shuffle(all_vertices.begin(), all_vertices.end(), rng);
    
    return std::vector<int>(all_vertices.begin(), all_vertices.begin() + count);
}

std::vector<int> BMSSPTestFramework::generateClusteredSources(const Graph& g, int count) {
    int n = g.getNumVertices();
    count = std::min(count, n);
    
    // Choose a random starting vertex
    std::uniform_int_distribution<int> vertex_dist(0, n - 1);
    int start = vertex_dist(rng);
    
    // Use BFS to find nearby vertices
    std::vector<bool> visited(n, false);
    std::queue<int> queue;
    std::vector<int> sources;
    
    queue.push(start);
    visited[start] = true;
    
    while (!queue.empty() && static_cast<int>(sources.size()) < count) {
        int current = queue.front();
        queue.pop();
        sources.push_back(current);
        
        // Add unvisited neighbors to queue
        for (const auto& edge : g.getConnections(current)) {
            if (!visited[edge.dest]) {
                visited[edge.dest] = true;
                queue.push(edge.dest);
            }
        }
    }
    
    // If we don't have enough sources, add random ones
    while (static_cast<int>(sources.size()) < count) {
        int candidate = vertex_dist(rng);
        if (std::find(sources.begin(), sources.end(), candidate) == sources.end()) {
            sources.push_back(candidate);
        }
    }
    
    return sources;
}

std::vector<int> BMSSPTestFramework::generateSpreadSources(const Graph& g, int count) {
    int n = g.getNumVertices();
    count = std::min(count, n);
    
    std::vector<int> sources;
    
    // Simple spread strategy: divide vertex space evenly
    for (int i = 0; i < count; ++i) {
        int vertex = (i * n) / count;
        sources.push_back(vertex);
    }
    
    return sources;
}

// Bound generation methods
double BMSSPTestFramework::generateBound(const Graph& g, const std::vector<int>& sources, BoundType type) {
    switch (type) {
        case BoundType::ZERO:
            return 0.0;
        case BoundType::VERY_SMALL:
            return 1e-6;
        case BoundType::INFINITE:
            return std::numeric_limits<double>::max();
        case BoundType::OPTIMAL: {
            return calculateOptimalBound(g, sources);
        }
        case BoundType::TIGHT: {
            double optimal = calculateOptimalBound(g, sources);
            return std::max(optimal * 1.1, 0.1); // 10% larger than optimal, min 0.1
        }
        case BoundType::LOOSE: {
            double optimal = calculateOptimalBound(g, sources);
            return std::max(optimal * 3.0, 1.0); // 3x larger than optimal, min 1.0
        }
        default:
            return 10.0;
    }
}

double BMSSPTestFramework::calculateOptimalBound(const Graph& g, const std::vector<int>& sources) {
    // Run reference Dijkstra to find actual shortest distances
    auto distances = runReferenceDijkstra(g, sources);
    
    // Find the maximum finite distance
    double max_distance = 0.0;
    bool found_finite = false;
    for (double dist : distances) {
        if (dist != std::numeric_limits<double>::max() && dist > max_distance) {
            max_distance = dist;
            found_finite = true;
        }
    }
    
    // If no finite distances found (disconnected graph), return a reasonable default
    if (!found_finite) {
        return 1.0; // Small positive value
    }
    
    // Ensure minimum bound of small positive value
    return std::max(max_distance, 0.1);
}

// Reference Dijkstra implementation
std::vector<double> BMSSPTestFramework::runReferenceDijkstra(const Graph& g, const std::vector<int>& sources) {
    int n = g.getNumVertices();
    std::vector<double> distances(n, std::numeric_limits<double>::max());
    
    using State = std::pair<double, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    
    // Initialize all sources
    for (int src : sources) {
        distances[src] = 0.0;
        pq.push({0.0, src});
    }
    
    while (!pq.empty()) {
        double dist = pq.top().first;
        int vertex = pq.top().second;
        pq.pop();
        
        if (dist > distances[vertex]) continue;
        
        for (const auto& edge : g.getConnections(vertex)) {
            double new_dist = dist + edge.weight;
            if (new_dist < distances[edge.dest]) {
                distances[edge.dest] = new_dist;
                pq.push({new_dist, edge.dest});
            }
        }
    }
    
    return distances;
}

// Test case generation
BMSSPTestCase BMSSPTestFramework::generateTestCase(const TestParameters& params) {
    BMSSPTestCase test_case(params.num_vertices);
    test_case.params = params;
    test_case.description = params.test_name;
    
    // Generate graph based on type
    switch (params.graph_type) {
        case GraphType::RANDOM_SPARSE:
            test_case.graph = generateRandomGraph(params.num_vertices, params.num_edges, params.weight_dist);
            break;
        case GraphType::TREE:
            test_case.graph = generateTreeGraph(params.num_vertices, params.weight_dist);
            break;
        case GraphType::CYCLE:
            test_case.graph = generateCycleGraph(params.num_vertices, params.weight_dist);
            break;
        case GraphType::GRID_2D: {
            int side = static_cast<int>(std::sqrt(params.num_vertices));
            test_case.graph = generateGrid2D(side, side, params.weight_dist);
            break;
        }
        case GraphType::STAR:
            test_case.graph = generateStarGraph(params.num_vertices, params.weight_dist);
            break;
        case GraphType::COMPLETE:
            test_case.graph = generateCompleteGraph(params.num_vertices, params.weight_dist);
            break;
        default:
            test_case.graph = generateRandomGraph(params.num_vertices, params.num_edges, params.weight_dist);
    }
    
    // Set graph parameters
    test_case.graph.calcK();
    test_case.graph.calcT();
    
    // Generate source set
    switch (params.source_method) {
        case SourceGenMethod::RANDOM:
            test_case.sources = generateRandomSources(test_case.graph, params.source_count);
            break;
        case SourceGenMethod::CLUSTERED:
            test_case.sources = generateClusteredSources(test_case.graph, params.source_count);
            break;
        case SourceGenMethod::SPREAD:
            test_case.sources = generateSpreadSources(test_case.graph, params.source_count);
            break;
        case SourceGenMethod::SINGLE_SOURCE:
            test_case.sources = {0};
            break;
        default:
            test_case.sources = generateRandomSources(test_case.graph, params.source_count);
    }
    
    // Generate bound
    test_case.bound = generateBound(test_case.graph, test_case.sources, params.bound_type);
    
    return test_case;
}

// Test execution
BMSSPTestOutput BMSSPTestFramework::executeBMSSP(const BMSSPTestCase& test_case) {
    BMSSPTestOutput output;
    
    try {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Prepare data structures
        Graph graph_copy = test_case.graph; // Create a copy since BMSSP might modify the graph
        int n = graph_copy.getNumVertices();
        
        std::vector<double> distances(n, std::numeric_limits<double>::max());
        std::vector<int> predecessors(n, -1);
        
        // Initialize source distances
        for (int src : test_case.sources) {
            distances[src] = 0.0;
        }
        
        // Run BMSSP
        int level = std::min(2, graph_copy.getT()); // Cap level to prevent exponential explosion
        BMSSPResult result = runBMSSP(graph_copy, distances, predecessors, 
                                     level, test_case.bound, test_case.sources);
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        
        // Fill output structure
        output.new_bound = result.new_bound;
        output.completed_vertices = result.completed_vertices;
        output.execution_success = true;
        output.execution_time_ms = duration.count() / 1000.0;
        output.recursive_calls = 0; // Would need instrumentation to track this
        output.total_vertices_processed = result.completed_vertices.size();
        
    } catch (const std::exception& e) {
        output.execution_success = false;
        output.error_message = e.what();
        output.new_bound = -1.0;
        output.execution_time_ms = 0.0;
    }
    
    return output;
}

// Verification methods
VerificationResult BMSSPTestFramework::verifyCorrectness(const BMSSPTestCase& test_case, const BMSSPTestOutput& output) {
    VerificationResult result;
    result.distances_correct = true;
    result.completeness_verified = true;
    result.bound_satisfaction = true;
    result.size_constraint_satisfied = true;
    result.max_distance_error = 0.0;
    
    if (!output.execution_success) {
        result.distances_correct = false;
        result.error_messages.push_back("Algorithm execution failed: " + output.error_message);
        return result;
    }
    
    // Verify bound constraint B' <= B
    if (output.new_bound > test_case.bound + 1e-9) { // Small tolerance for floating point
        result.bound_satisfaction = false;
        result.error_messages.push_back("New bound exceeds original bound");
    }
    
    // Run reference algorithm for comparison
    auto reference_distances = runReferenceDijkstra(test_case.graph, test_case.sources);
    
    // Verify distances for completed vertices
    for (int vertex : output.completed_vertices) {
        // Check that the vertex is reachable
        if (reference_distances[vertex] == std::numeric_limits<double>::max()) {
            result.completeness_verified = false;
            result.error_messages.push_back("Completed vertex " + std::to_string(vertex) + " is unreachable");
            continue;
        }
        
        // Check that distance is within bound
        if (reference_distances[vertex] > output.new_bound + 1e-9) {
            result.completeness_verified = false;
            result.error_messages.push_back("Completed vertex " + std::to_string(vertex) + 
                                           " has distance exceeding new bound");
        }
    }
    
    // Verify that all vertices with distance < B' are included
    for (int v = 0; v < test_case.graph.getNumVertices(); ++v) {
        if (reference_distances[v] < output.new_bound - 1e-9) {
            bool found = std::find(output.completed_vertices.begin(), 
                                 output.completed_vertices.end(), v) != output.completed_vertices.end();
            if (!found) {
                result.completeness_verified = false;
                result.error_messages.push_back("Vertex " + std::to_string(v) + 
                                               " should be completed but is missing");
            }
        }
    }
    
    return result;
}

// Test suite generators
std::vector<BMSSPTestCase> BMSSPTestFramework::generateCorrectnessTests() {
    std::vector<BMSSPTestCase> tests;
    
    // Small graph tests with known solutions
    TestParameters params;
    params.num_vertices = 10;
    params.num_edges = 20;
    params.graph_type = GraphType::RANDOM_SPARSE;
    params.weight_dist = WeightDistribution::UNIT_WEIGHTS;
    params.source_method = SourceGenMethod::SINGLE_SOURCE;
    params.source_count = 1;
    params.bound_type = BoundType::LOOSE;
    params.k_param = 2;
    params.t_param = 2;
    params.test_name = "Small random graph with unit weights";
    
    tests.push_back(generateTestCase(params));
    
    // Tree test
    params.graph_type = GraphType::TREE;
    params.test_name = "Tree graph test";
    tests.push_back(generateTestCase(params));
    
    // Cycle test
    params.graph_type = GraphType::CYCLE;
    params.test_name = "Cycle graph test";
    tests.push_back(generateTestCase(params));
    
    return tests;
}

std::vector<BMSSPTestCase> BMSSPTestFramework::generateEdgeCaseTests() {
    std::vector<BMSSPTestCase> tests;
    
    // Single vertex test
    TestParameters params;
    params.num_vertices = 1;
    params.num_edges = 0;
    params.graph_type = GraphType::RANDOM_SPARSE;
    params.weight_dist = WeightDistribution::UNIT_WEIGHTS;
    params.source_method = SourceGenMethod::SINGLE_SOURCE;
    params.source_count = 1;
    params.bound_type = BoundType::LOOSE;
    params.k_param = 1;
    params.t_param = 1;
    params.test_name = "Single vertex graph";
    
    tests.push_back(generateTestCase(params));
    
    // Zero bound test
    params.num_vertices = 5;
    params.num_edges = 8;
    params.bound_type = BoundType::ZERO;
    params.test_name = "Zero bound test";
    tests.push_back(generateTestCase(params));
    
    return tests;
}

// Simple test execution
void BMSSPTestFramework::runCorrectnessTestSuite() {
    std::cout << "=== Running BMSSP Correctness Test Suite ===" << std::endl;
    
    auto test_cases = generateCorrectnessTests();
    std::vector<VerificationResult> results;
    
    for (size_t i = 0; i < test_cases.size(); ++i) {
        std::cout << "\nTest " << (i + 1) << ": " << test_cases[i].description << std::endl;
        
        auto output = executeBMSSP(test_cases[i]);
        auto verification = verifyCorrectness(test_cases[i], output);
        
        results.push_back(verification);
        
        if (verification.distances_correct && verification.completeness_verified && 
            verification.bound_satisfaction) {
            std::cout << "✓ PASSED" << std::endl;
        } else {
            std::cout << "✗ FAILED" << std::endl;
            for (const auto& error : verification.error_messages) {
                std::cout << "  Error: " << error << std::endl;
            }
        }
        
        std::cout << "  Execution time: " << std::fixed << std::setprecision(2) 
                  << output.execution_time_ms << " ms" << std::endl;
        std::cout << "  Completed vertices: " << output.completed_vertices.size() << std::endl;
        std::cout << "  New bound: " << output.new_bound << std::endl;
    }
    
    printTestSummary(results);
}

void BMSSPTestFramework::printTestSummary(const std::vector<VerificationResult>& results) {
    int passed = 0;
    int total = results.size();
    
    for (const auto& result : results) {
        if (result.distances_correct && result.completeness_verified && result.bound_satisfaction) {
            passed++;
        }
    }
    
    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Total tests: " << total << std::endl;
    std::cout << "Passed: " << passed << std::endl;
    std::cout << "Failed: " << (total - passed) << std::endl;
    std::cout << "Success rate: " << std::fixed << std::setprecision(1) 
              << (100.0 * passed / total) << "%" << std::endl;
}
