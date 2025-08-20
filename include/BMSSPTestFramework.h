#ifndef BMSSP_TEST_FRAMEWORK_H
#define BMSSP_TEST_FRAMEWORK_H

#include "Graph.h"
#include "BMSSP.h"
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <unordered_set>

// Test case structures
enum class GraphType {
    RANDOM_SPARSE,
    RANDOM_DENSE, 
    TREE,
    CYCLE,
    GRID_2D,
    STAR,
    BIPARTITE,
    LAYERED,
    COMPLETE,
    DISCONNECTED
};

enum class WeightDistribution {
    UNIFORM,
    EXPONENTIAL,
    NORMAL_TRUNCATED,
    INTEGER_SMALL,
    INTEGER_LARGE,
    UNIT_WEIGHTS,
    BINARY_WEIGHTS,
    POWER_OF_TWO
};

enum class SourceGenMethod {
    RANDOM,
    CLUSTERED,
    SPREAD,
    CORNER_CASES,
    SINGLE_SOURCE,
    ALL_VERTICES
};

enum class BoundType {
    TIGHT,
    LOOSE,
    OPTIMAL,
    INFINITE,
    ZERO,
    VERY_SMALL
};

struct TestParameters {
    int num_vertices;
    int num_edges;
    GraphType graph_type;
    WeightDistribution weight_dist;
    SourceGenMethod source_method;
    int source_count;
    BoundType bound_type;
    int k_param;
    int t_param;
    std::string test_name;
    bool ensure_connectivity = false;  // New parameter to guarantee connectivity
    bool is_directed = true;          // New parameter to specify if graph should be directed
};

struct BMSSPTestCase {
    Graph graph;
    std::vector<int> sources;
    double bound;
    TestParameters params;
    std::string description;
    
    // Default constructor
    BMSSPTestCase() : graph(1), bound(0.0) {}
    
    // Constructor with graph size
    BMSSPTestCase(int n) : graph(n), bound(0.0) {}
};

struct BMSSPTestOutput {
    double new_bound;
    std::vector<int> completed_vertices;
    bool execution_success;
    double execution_time_ms;
    int recursive_calls;
    int total_vertices_processed;
    std::string error_message;
};

struct VerificationResult {
    bool distances_correct;
    bool completeness_verified;
    bool bound_satisfaction;
    bool size_constraint_satisfied;
    std::vector<std::string> error_messages;
    double max_distance_error;
};

struct PerformanceMeasurement {
    double wall_clock_time_ms;
    double cpu_time_ms;
    size_t memory_peak_kb;
    int recursive_call_count;
    int dijkstra_calls;
    double pivot_finding_time_ms;
    int heap_operations;
};

struct AlgorithmComparison {
    double bmssp_time_ms;
    double dijkstra_time_ms;
    double speedup_ratio;
    bool correctness_match;
    int vertices_processed_bmssp;
    int vertices_processed_dijkstra;
};

// Main test framework class
class BMSSPTestFramework {
private:
    std::mt19937 rng;
    
    // Graph generation helpers
    Graph generateRandomGraph(int n, int m, WeightDistribution weight_dist);
    Graph generateTreeGraph(int n, WeightDistribution weight_dist);
    Graph generateCycleGraph(int n, WeightDistribution weight_dist);
    Graph generateGrid2D(int rows, int cols, WeightDistribution weight_dist);
    Graph generateStarGraph(int n, WeightDistribution weight_dist);
    Graph generateCompleteGraph(int n, WeightDistribution weight_dist);
    Graph generateDisconnectedGraph(int n, int components, WeightDistribution weight_dist);
    
    // New connected graph generation function
    Graph generateConnectedGraph(int num_vertices, int num_edges, WeightDistribution dist, bool is_directed = true);
    
    // Weight generation helpers
    double generateWeight(WeightDistribution dist);
    std::vector<double> generateWeights(int count, WeightDistribution dist);
    
    // Source set generation helpers
    std::vector<int> generateRandomSources(const Graph& g, int count);
    std::vector<int> generateClusteredSources(const Graph& g, int count);
    std::vector<int> generateSpreadSources(const Graph& g, int count);
    std::vector<int> generateCornerCaseSources(const Graph& g, SourceGenMethod method);
    
    // Bound generation helpers
    double generateBound(const Graph& g, const std::vector<int>& sources, BoundType type);
    double calculateOptimalBound(const Graph& g, const std::vector<int>& sources);
    
public:
    BMSSPTestFramework(unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count());
    
    // Reference implementation for verification (moved to public for performance tests)
    std::vector<double> runReferenceDijkstra(const Graph& g, const std::vector<int>& sources);
    
    // Main test case generation
    BMSSPTestCase generateTestCase(const TestParameters& params);
    std::vector<BMSSPTestCase> generateTestSuite();
    
    // Test execution
    BMSSPTestOutput executeBMSSP(const BMSSPTestCase& test_case);
    
    // Verification methods
    VerificationResult verifyCorrectness(const BMSSPTestCase& test_case, const BMSSPTestOutput& output);
    PerformanceMeasurement measurePerformance(const BMSSPTestCase& test_case);
    AlgorithmComparison compareWithDijkstra(const BMSSPTestCase& test_case);
    
    // Specialized test generators
    std::vector<BMSSPTestCase> generateCorrectnessTests();
    std::vector<BMSSPTestCase> generatePerformanceTests();
    std::vector<BMSSPTestCase> generateEdgeCaseTests();
    std::vector<BMSSPTestCase> generateScalingTests();
    
    // Test suite execution
    void runComprehensiveTestSuite();
    void runCorrectnessTestSuite();
    void runPerformanceTestSuite();
    void runEdgeCaseTestSuite();
    
    // Reporting
    void generateTestReport(const std::vector<VerificationResult>& results,
                           const std::vector<PerformanceMeasurement>& performance);
    void printTestSummary(const std::vector<VerificationResult>& results);
};

// Utility functions for test case analysis
namespace BMSSPTestUtils {
    bool isValidGraph(const Graph& g);
    bool isValidSourceSet(const Graph& g, const std::vector<int>& sources);
    double calculateGraphDiameter(const Graph& g);
    int calculateGraphComponents(const Graph& g);
    std::string formatTestResults(const VerificationResult& result);
    std::string formatPerformanceResults(const PerformanceMeasurement& perf);
}

// Test case factories for specific scenarios
namespace BMSSPTestCaseFactory {
    BMSSPTestCase createMinimalTest();
    BMSSPTestCase createSingleVertexTest();
    BMSSPTestCase createDisconnectedTest();
    BMSSPTestCase createLargeScaleTest(int n);
    BMSSPTestCase createPrecisionTest();
    BMSSPTestCase createParameterEdgeTest(int k, int t);
    BMSSPTestCase createZeroBoundTest();
    BMSSPTestCase createInfiniteBoundTest();
}

#endif // BMSSP_TEST_FRAMEWORK_H
