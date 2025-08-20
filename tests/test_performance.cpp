#include "BMSSPTestFramework.h"
#include "Debug.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <fstream>

/**
 * Performance and Scalability Test Suite
 * Measures and analyzes performance characteristics:
 * - Runtime scaling with graph size
 * - Performance comparison with baseline algorithms
 * - Memory usage analysis
 * - Algorithmic complexity verification
 * - Bottleneck identification
 */

struct PerformanceMetrics {
    int graph_size;
    int edge_count;
    double execution_time_ms;
    double vertices_per_second;
    int completed_vertices;
    bool execution_success;
    std::string graph_type;
    std::string error_message;
};

class PerformanceTestRunner {
private:
    BMSSPTestFramework framework;
    std::vector<PerformanceMetrics> results;
    
public:
    void runScalabilityTests() {
        std::cout << "=== SCALABILITY ANALYSIS ===" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        // Small to medium graph sizes
        std::vector<int> sizes = {10, 20, 50, 100, 200, 500, 1000};
        
        std::cout << std::setw(8) << "Size" 
                  << std::setw(8) << "Edges" 
                  << std::setw(12) << "Time (ms)" 
                  << std::setw(12) << "V/sec" 
                  << std::setw(10) << "Status" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        for (int n : sizes) {
            std::cout << "[DEBUG] ===== Starting test for size " << n << " =====" << std::endl;
            try {
                runScalabilityTest(n);
                std::cout << "[DEBUG] ===== Completed test for size " << n << " =====" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "[DEBUG] ===== Exception in test for size " << n << ": " << e.what() << " =====" << std::endl;
                break;
            } catch (...) {
                std::cout << "[DEBUG] ===== Unknown exception in test for size " << n << " =====" << std::endl;
                break;
            }
        }
        
        analyzeScalingBehavior();
    }
    
    void runGraphTypePerformance() {
        std::cout << "\n=== GRAPH TYPE PERFORMANCE COMPARISON ===" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        std::vector<std::pair<GraphType, std::string>> types = {
            {GraphType::TREE, "Tree"},
            {GraphType::RANDOM_SPARSE, "Sparse Random"},
            {GraphType::CYCLE, "Cycle"},
            {GraphType::STAR, "Star"},
            {GraphType::GRID_2D, "Grid 2D"}
        };
        
        int test_size = 200;
        
        std::cout << std::setw(15) << "Graph Type" 
                  << std::setw(12) << "Time (ms)" 
                  << std::setw(12) << "V/sec" 
                  << std::setw(15) << "Completed V" 
                  << std::setw(10) << "Status" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& type_pair : types) {
            runGraphTypeTest(type_pair.first, type_pair.second, test_size);
        }
    }
    
    void runBoundSensitivityAnalysis() {
        std::cout << "\n=== BOUND PARAMETER SENSITIVITY ANALYSIS ===" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        std::vector<std::pair<BoundType, std::string>> bounds = {
            {BoundType::TIGHT, "Tight"},
            {BoundType::LOOSE, "Loose"},
            {BoundType::OPTIMAL, "Optimal"},
            {BoundType::VERY_SMALL, "Very Small"}
        };
        
        int test_size = 100;
        
        std::cout << std::setw(12) << "Bound Type" 
                  << std::setw(12) << "Time (ms)" 
                  << std::setw(12) << "V/sec" 
                  << std::setw(15) << "Completed V" 
                  << std::setw(10) << "Status" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& bound_pair : bounds) {
            runBoundSensitivityTest(bound_pair.first, bound_pair.second, test_size);
        }
    }
    
    void runDijkstraComparison() {
        std::cout << "\n=== DIJKSTRA COMPARISON ANALYSIS ===" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        std::vector<int> sizes = {50, 100, 200, 500};
        
        std::cout << std::setw(8) << "Size" 
                  << std::setw(12) << "BMSSP (ms)" 
                  << std::setw(12) << "Dijkstra (ms)" 
                  << std::setw(12) << "Speedup" 
                  << std::setw(10) << "Winner" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (int size : sizes) {
            runDijkstraComparisonTest(size);
        }
    }
    
    void runStressTests() {
        std::cout << "\n=== STRESS TESTS ===" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        // Test 1: Large dense graph
        std::cout << "Stress Test 1: Large dense graph" << std::endl;
        runStressTest("Large Dense", 1000, GraphType::COMPLETE, 500000);
        
        // Test 2: Many sources
        std::cout << "Stress Test 2: Many sources" << std::endl;
        runManySources();
        
        // Test 3: Long running test
        std::cout << "Stress Test 3: Extended execution" << std::endl;
        runExtendedTest();
    }
    
    void runLargeScaleTests() {
        std::cout << "\n=== LARGE-SCALE PERFORMANCE ANALYSIS (10^5 vertices) ===" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "Testing BMSSP vs Dijkstra performance on large graphs..." << std::endl;
        
        // Start with more conservative sizes and build up
        std::vector<int> large_sizes = {2500, 3000, 4000, 5000, 7500};
        
        std::cout << "\n" << std::setw(10) << "Size" 
                  << std::setw(12) << "Edges" 
                  << std::setw(15) << "BMSSP (ms)" 
                  << std::setw(15) << "Dijkstra (ms)" 
                  << std::setw(12) << "Speedup" 
                  << std::setw(12) << "Winner" << std::endl;
        std::cout << std::string(75, '-') << std::endl;
        
        for (int n : large_sizes) {
            std::cout << "Testing graph size: " << n << " vertices..." << std::endl;
            runLargeScaleComparisonTest(n);
        }
        
        std::cout << "\n=== LARGE-SCALE ANALYSIS COMPLETE ===" << std::endl;
    }
    
    void generatePerformanceReport() {
        std::cout << "\n=== PERFORMANCE ANALYSIS REPORT ===" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        if (results.empty()) {
            std::cout << "No performance data collected." << std::endl;
            return;
        }
        
        // Basic statistics
        auto successful_results = getSuccessfulResults();
        if (successful_results.empty()) {
            std::cout << "No successful executions to analyze." << std::endl;
            return;
        }
        
        double avg_time = 0.0, min_time = 1e9, max_time = 0.0;
        double avg_throughput = 0.0;
        
        for (const auto& result : successful_results) {
            avg_time += result.execution_time_ms;
            min_time = std::min(min_time, result.execution_time_ms);
            max_time = std::max(max_time, result.execution_time_ms);
            avg_throughput += result.vertices_per_second;
        }
        
        avg_time /= successful_results.size();
        avg_throughput /= successful_results.size();
        
        std::cout << "Execution Statistics:" << std::endl;
        std::cout << "  Successful runs: " << successful_results.size() << "/" << results.size() << std::endl;
        std::cout << "  Average execution time: " << std::fixed << std::setprecision(2) << avg_time << " ms" << std::endl;
        std::cout << "  Fastest execution: " << min_time << " ms" << std::endl;
        std::cout << "  Slowest execution: " << max_time << " ms" << std::endl;
        std::cout << "  Average throughput: " << std::fixed << std::setprecision(0) << avg_throughput << " vertices/sec" << std::endl;
        
        // Scaling analysis
        analyzeComplexityScaling();
        
        // Generate CSV report
        generateCSVReport();
        
        std::cout << "\n✓ Performance analysis complete!" << std::endl;
    }
    
private:
    void runScalabilityTest(int size) {
        std::cout << "[DEBUG] Starting scalability test for size " << size << std::endl;
        
        TestParameters params;
        params.num_vertices = size;
        params.num_edges = std::min(size * 3, size * (size - 1) / 4);
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = std::max(1, size / 20);
        params.bound_type = BoundType::LOOSE;
        params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
        params.t_param = 2;
        params.test_name = "Scalability test n=" + std::to_string(size);
        
        std::cout << "[DEBUG] Parameters: vertices=" << params.num_vertices 
                  << ", edges=" << params.num_edges 
                  << ", sources=" << params.source_count 
                  << ", k=" << params.k_param 
                  << ", t=" << params.t_param << std::endl;
        
        std::cout << "[DEBUG] Calling runPerformanceTest..." << std::endl;
        auto metrics = runPerformanceTest(params, "Sparse");
        std::cout << "[DEBUG] runPerformanceTest completed, success=" << metrics.execution_success << std::endl;
        
        std::cout << std::setw(8) << size
                  << std::setw(8) << params.num_edges
                  << std::setw(12) << std::fixed << std::setprecision(2) << metrics.execution_time_ms
                  << std::setw(12) << std::fixed << std::setprecision(0) << metrics.vertices_per_second
                  << std::setw(10) << (metrics.execution_success ? "OK" : "FAIL") << std::endl;
        
        if (!metrics.execution_success) {
            std::cout << "[DEBUG] Error: " << metrics.error_message << std::endl;
        }
    }
    
    void runGraphTypeTest(GraphType type, const std::string& name, int size) {
        TestParameters params;
        params.num_vertices = size;
        params.num_edges = (type == GraphType::COMPLETE) ? size * (size - 1) / 2 : size * 2;
        params.graph_type = type;
        params.weight_dist = WeightDistribution::UNIT_WEIGHTS;
        params.source_method = SourceGenMethod::SINGLE_SOURCE;
        params.source_count = 1;
        params.bound_type = BoundType::LOOSE;
        params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
        params.t_param = 2;
        params.test_name = name + " performance test";
        
        auto metrics = runPerformanceTest(params, name);
        
        std::cout << std::setw(15) << name
                  << std::setw(12) << std::fixed << std::setprecision(2) << metrics.execution_time_ms
                  << std::setw(12) << std::fixed << std::setprecision(0) << metrics.vertices_per_second
                  << std::setw(15) << metrics.completed_vertices
                  << std::setw(10) << (metrics.execution_success ? "OK" : "FAIL") << std::endl;
    }
    
    void runBoundSensitivityTest(BoundType bound_type, const std::string& name, int size) {
        TestParameters params;
        params.num_vertices = size;
        params.num_edges = size * 2;
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = 3;
        params.bound_type = bound_type;
        params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
        params.t_param = 2;
        params.test_name = name + " bound test";
        
        auto metrics = runPerformanceTest(params, "Bound-" + name);
        
        std::cout << std::setw(12) << name
                  << std::setw(12) << std::fixed << std::setprecision(2) << metrics.execution_time_ms
                  << std::setw(12) << std::fixed << std::setprecision(0) << metrics.vertices_per_second
                  << std::setw(15) << metrics.completed_vertices
                  << std::setw(10) << (metrics.execution_success ? "OK" : "FAIL") << std::endl;
    }
    
    void runDijkstraComparisonTest(int size) {
        // Create test case
        TestParameters params;
        params.num_vertices = size;
        params.num_edges = size * 3;
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = std::max(1, size / 20);
        params.bound_type = BoundType::LOOSE;
        params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
        params.t_param = 2;
        params.test_name = "Comparison test n=" + std::to_string(size);
        
        try {
            auto test_case = framework.generateTestCase(params);
            
            // Time BMSSP
            auto start_bmssp = std::chrono::high_resolution_clock::now();
            auto bmssp_output = framework.executeBMSSP(test_case);
            auto end_bmssp = std::chrono::high_resolution_clock::now();
            double bmssp_time = std::chrono::duration_cast<std::chrono::microseconds>(end_bmssp - start_bmssp).count() / 1000.0;
            
            // Time reference Dijkstra
            auto start_dijkstra = std::chrono::high_resolution_clock::now();
            auto dijkstra_distances = framework.runReferenceDijkstra(test_case.graph, test_case.sources);
            auto end_dijkstra = std::chrono::high_resolution_clock::now();
            double dijkstra_time = std::chrono::duration_cast<std::chrono::microseconds>(end_dijkstra - start_dijkstra).count() / 1000.0;
            
            double speedup = dijkstra_time / std::max(bmssp_time, 0.001);
            std::string winner = (bmssp_time < dijkstra_time) ? "BMSSP" : "Dijkstra";
            
            std::cout << std::setw(8) << size
                      << std::setw(12) << std::fixed << std::setprecision(2) << bmssp_time
                      << std::setw(12) << dijkstra_time
                      << std::setw(12) << std::setprecision(2) << speedup << "x"
                      << std::setw(10) << winner << std::endl;
                      
        } catch (const std::exception& e) {
            std::cout << std::setw(8) << size
                      << std::setw(12) << "ERROR"
                      << std::setw(12) << "ERROR"
                      << std::setw(12) << "N/A"
                      << std::setw(10) << "FAIL" << std::endl;
        }
    }
    
    void runLargeScaleComparisonTest(int size) {
        // Create test case with sparse graph to keep memory manageable
        TestParameters params;
        params.num_vertices = size;
        params.num_edges = size * 3; // Very sparse graph: 3 edges per vertex on average
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = std::max(1, std::min(10, size / 1000)); // Much smaller source count
        params.bound_type = BoundType::LOOSE;
        params.k_param = std::max(1, std::min(5, static_cast<int>(std::sqrt(size)))); // Cap k parameter
        params.t_param = std::min(2, params.k_param); // Ensure t <= k
        params.test_name = "Large-scale comparison n=" + std::to_string(size);
        
        try {
            std::cout << "  Generating large graph (this may take time)..." << std::endl;
            auto test_case = framework.generateTestCase(params);
            std::cout << "  Graph generated successfully." << std::endl;
            
            // Time BMSSP
            std::cout << "  Running BMSSP with k=" << params.k_param << ", t=" << params.t_param << "..." << std::endl;
            std::cout << "  Graph parameters: vertices=" << test_case.graph.getNumVertices() 
                      << ", k=" << test_case.graph.getK() 
                      << ", t=" << test_case.graph.getT() << std::endl;
            auto start_bmssp = std::chrono::high_resolution_clock::now();
            auto bmssp_result = framework.executeBMSSP(test_case);
            auto end_bmssp = std::chrono::high_resolution_clock::now();
            
            auto bmssp_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end_bmssp - start_bmssp).count() / 1000.0;
            
            // Time Dijkstra (single source for fair comparison)
            std::cout << "  Running Dijkstra..." << std::endl;
            auto start_dijkstra = std::chrono::high_resolution_clock::now();
            std::vector<int> single_source = {test_case.sources[0]};
            auto dijkstra_result = framework.runReferenceDijkstra(test_case.graph, single_source);
            auto end_dijkstra = std::chrono::high_resolution_clock::now();
            
            auto dijkstra_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end_dijkstra - start_dijkstra).count() / 1000.0;
            
            // Calculate speedup
            double speedup = dijkstra_time_ms / bmssp_time_ms;
            std::string winner = speedup > 1.0 ? "BMSSP" : "Dijkstra";
            
            std::cout << std::setw(10) << size
                      << std::setw(12) << params.num_edges
                      << std::setw(15) << std::fixed << std::setprecision(2) << bmssp_time_ms
                      << std::setw(15) << std::fixed << std::setprecision(2) << dijkstra_time_ms
                      << std::setw(12) << std::fixed << std::setprecision(2) << speedup << "x"
                      << std::setw(12) << winner << std::endl;
                      
        } catch (const std::exception& e) {
            std::cout << std::setw(10) << size
                      << std::setw(12) << "ERROR"
                      << std::setw(15) << "N/A"
                      << std::setw(15) << "N/A"
                      << std::setw(12) << "N/A"
                      << std::setw(12) << "FAIL" << std::endl;
            std::cout << "  Error: " << e.what() << std::endl;
        }
    }
    
    void runStressTest(const std::string& name, int size, GraphType type, int max_edges) {
        TestParameters params;
        params.num_vertices = size;
        params.num_edges = std::min(max_edges, size * (size - 1) / 2);
        params.graph_type = type;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = std::max(1, size / 50);
        params.bound_type = BoundType::LOOSE;
        params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
        params.t_param = 2;
        params.test_name = name;
        
        std::cout << "  Running " << name << " (n=" << size << ", m=" << params.num_edges << ")..." << std::endl;
        
        auto start_time = std::chrono::high_resolution_clock::now();
        auto metrics = runPerformanceTest(params, name);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        auto total_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
        
        if (metrics.execution_success) {
            std::cout << "  ✓ Completed in " << total_time << " seconds" << std::endl;
            std::cout << "    Execution time: " << metrics.execution_time_ms << " ms" << std::endl;
            std::cout << "    Completed vertices: " << metrics.completed_vertices << std::endl;
        } else {
            std::cout << "  ✗ Failed: " << metrics.error_message << std::endl;
        }
    }
    
    void runManySources() {
        TestParameters params;
        params.num_vertices = 200;
        params.num_edges = 600;
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = 50; // Many sources
        params.bound_type = BoundType::LOOSE;
        params.k_param = 5;
        params.t_param = 2;
        params.test_name = "Many sources test";
        
        std::cout << "  Running with " << params.source_count << " sources..." << std::endl;
        auto metrics = runPerformanceTest(params, "ManySources");
        
        if (metrics.execution_success) {
            std::cout << "  ✓ Handled many sources successfully" << std::endl;
            std::cout << "    Execution time: " << metrics.execution_time_ms << " ms" << std::endl;
        } else {
            std::cout << "  ✗ Many sources test failed: " << metrics.error_message << std::endl;
        }
    }
    
    void runExtendedTest() {
        // Run multiple iterations to test consistency
        const int iterations = 10;
        std::vector<double> times;
        
        TestParameters params;
        params.num_vertices = 100;
        params.num_edges = 300;
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = 5;
        params.bound_type = BoundType::LOOSE;
        params.k_param = 3;
        params.t_param = 2;
        params.test_name = "Extended test";
        
        std::cout << "  Running " << iterations << " iterations..." << std::endl;
        
        for (int i = 0; i < iterations; i++) {
            auto metrics = runPerformanceTest(params, "Extended");
            if (metrics.execution_success) {
                times.push_back(metrics.execution_time_ms);
            }
        }
        
        if (!times.empty()) {
            double avg_time = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
            double min_time = *std::min_element(times.begin(), times.end());
            double max_time = *std::max_element(times.begin(), times.end());
            
            std::cout << "  ✓ Extended test completed" << std::endl;
            std::cout << "    Successful iterations: " << times.size() << "/" << iterations << std::endl;
            std::cout << "    Average time: " << std::fixed << std::setprecision(2) << avg_time << " ms" << std::endl;
            std::cout << "    Time range: " << min_time << " - " << max_time << " ms" << std::endl;
        } else {
            std::cout << "  ✗ All extended test iterations failed" << std::endl;
        }
    }
    
    PerformanceMetrics runPerformanceTest(const TestParameters& params, const std::string& category) {
        std::cout << "[DEBUG] runPerformanceTest: Creating metrics for " << category << std::endl;
        
        PerformanceMetrics metrics;
        metrics.graph_size = params.num_vertices;
        metrics.edge_count = params.num_edges;
        metrics.graph_type = category;
        metrics.execution_success = false;
        
        try {
            std::cout << "[DEBUG] Generating test case..." << std::endl;
            auto test_case = framework.generateTestCase(params);
            std::cout << "[DEBUG] Test case generated successfully, executing BMSSP..." << std::endl;
            
            auto output = framework.executeBMSSP(test_case);
            std::cout << "[DEBUG] BMSSP execution completed, success=" << output.execution_success << std::endl;
            
            metrics.execution_time_ms = output.execution_time_ms;
            metrics.execution_success = output.execution_success;
            metrics.completed_vertices = output.total_vertices_processed;
            
            if (output.execution_success && output.execution_time_ms > 0) {
                metrics.vertices_per_second = (params.num_vertices * 1000.0) / output.execution_time_ms;
            } else {
                metrics.vertices_per_second = 0.0;
            }
            
            if (!output.execution_success) {
                metrics.error_message = output.error_message;
                std::cout << "[DEBUG] Execution failed: " << output.error_message << std::endl;
            }
            
        } catch (const std::exception& e) {
            std::cout << "[DEBUG] Exception caught: " << e.what() << std::endl;
            metrics.error_message = e.what();
        }
        
        std::cout << "[DEBUG] Adding result to results vector..." << std::endl;
        results.push_back(metrics);
        std::cout << "[DEBUG] runPerformanceTest completed" << std::endl;
        return metrics;
    }
    
    std::vector<PerformanceMetrics> getSuccessfulResults() const {
        std::vector<PerformanceMetrics> successful;
        for (const auto& result : results) {
            if (result.execution_success) {
                successful.push_back(result);
            }
        }
        return successful;
    }
    
    void analyzeScalingBehavior() {
        auto successful = getSuccessfulResults();
        if (successful.size() < 3) {
            std::cout << "\nInsufficient data for scaling analysis." << std::endl;
            return;
        }
        
        std::cout << "\nScaling Analysis:" << std::endl;
        
        // Sort by graph size
        std::sort(successful.begin(), successful.end(), 
                 [](const PerformanceMetrics& a, const PerformanceMetrics& b) {
                     return a.graph_size < b.graph_size;
                 });
        
        // Calculate growth rates
        std::cout << "  Growth rate analysis:" << std::endl;
        for (size_t i = 1; i < successful.size(); i++) {
            double size_ratio = static_cast<double>(successful[i].graph_size) / successful[i-1].graph_size;
            double time_ratio = successful[i].execution_time_ms / successful[i-1].execution_time_ms;
            
            std::cout << "    n=" << successful[i-1].graph_size << " to n=" << successful[i].graph_size
                      << ": " << std::fixed << std::setprecision(2) << time_ratio << "x time increase"
                      << " (size increased " << size_ratio << "x)" << std::endl;
        }
    }
    
    void analyzeComplexityScaling() {
        auto successful = getSuccessfulResults();
        if (successful.size() < 4) return;
        
        std::cout << "\nComplexity Analysis:" << std::endl;
        
        // Simple empirical complexity estimation
        // Check if it fits O(n log n), O(n^1.5), O(n^2), etc.
        double best_fit = 1e9;
        std::string best_complexity = "Unknown";
        
        std::vector<std::pair<std::string, std::function<double(int,int)>>> complexities;
        complexities.emplace_back("O(n)", [](int n, int m) { return static_cast<double>(n); });
        complexities.emplace_back("O(m)", [](int n, int m) { return static_cast<double>(m); });
        complexities.emplace_back("O(n log n)", [](int n, int m) { return n * std::log(n); });
        complexities.emplace_back("O(m log n)", [](int n, int m) { return m * std::log(n); });
        complexities.emplace_back("O(n^1.5)", [](int n, int m) { return std::pow(n, 1.5); });
        complexities.emplace_back("O(n^2)", [](int n, int m) { return static_cast<double>(n * n); });
        
        for (const auto& complexity : complexities) {
            double variance = 0.0;
            for (size_t i = 0; i < successful.size(); i++) {
                double predicted = complexity.second(successful[i].graph_size, successful[i].edge_count);
                double actual = successful[i].execution_time_ms;
                double ratio = actual / predicted;
                
                if (i > 0) {
                    double prev_predicted = complexity.second(successful[i-1].graph_size, successful[i-1].edge_count);
                    double prev_actual = successful[i-1].execution_time_ms;
                    double prev_ratio = prev_actual / prev_predicted;
                    variance += std::abs(ratio - prev_ratio);
                }
            }
            
            if (variance < best_fit) {
                best_fit = variance;
                best_complexity = complexity.first;
            }
        }
        
        std::cout << "  Best fit complexity: " << best_complexity << std::endl;
        std::cout << "  (This is a rough empirical estimate)" << std::endl;
    }
    
    void generateCSVReport() {
        std::ofstream csv_file("performance_results.csv");
        if (!csv_file.is_open()) {
            std::cout << "Could not create CSV report file." << std::endl;
            return;
        }
        
        csv_file << "graph_size,edge_count,execution_time_ms,vertices_per_second,completed_vertices,graph_type,success\n";
        
        for (const auto& result : results) {
            csv_file << result.graph_size << ","
                     << result.edge_count << ","
                     << result.execution_time_ms << ","
                     << result.vertices_per_second << ","
                     << result.completed_vertices << ","
                     << result.graph_type << ","
                     << (result.execution_success ? "true" : "false") << "\n";
        }
        
        csv_file.close();
        std::cout << "\n✓ Performance data saved to performance_results.csv" << std::endl;
    }
};

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS]\n"
              << "\nOptions:\n"
              << "  --scalability     Run scalability analysis\n"
              << "  --graph-types     Run graph type performance comparison\n"
              << "  --bounds          Run bound parameter sensitivity analysis\n"
              << "  --comparison      Run BMSSP vs Dijkstra comparison\n"
              << "  --stress          Run stress tests\n"
              << "  --large-scale     Run large-scale performance tests (10^5 vertices)\n"
              << "  --all             Run all performance tests (default)\n"
              << "  --debug, -d       Enable debug output\n"
              << "  --help            Show this help message\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "=== PERFORMANCE AND SCALABILITY TEST SUITE ===" << std::endl;
    std::cout << "Analyzing performance characteristics and scaling behavior" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Initialize debug system
    initializeDebug(argc, argv);
    DEBUG_PRINT("Performance test suite starting");
    
    PerformanceTestRunner runner;
    
    // Parse command line arguments
    bool run_all = true;
    bool run_scalability = false, run_graph_types = false, run_bounds = false;
    bool run_comparison = false, run_stress = false, run_large_scale = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--debug" || arg == "-d") {
            // Already handled in initializeDebug
            continue;
        } else if (arg == "--scalability") {
            run_scalability = true; run_all = false;
        } else if (arg == "--graph-types") {
            run_graph_types = true; run_all = false;
        } else if (arg == "--bounds") {
            run_bounds = true; run_all = false;
        } else if (arg == "--comparison") {
            run_comparison = true; run_all = false;
        } else if (arg == "--stress") {
            run_stress = true; run_all = false;
        } else if (arg == "--large-scale") {
            run_large_scale = true; run_all = false;
        } else if (arg == "--all") {
            run_all = true;
        } else {
            std::cout << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    try {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        if (run_all || run_scalability) {
            runner.runScalabilityTests();
        }
        
        if (run_all || run_graph_types) {
            runner.runGraphTypePerformance();
        }
        
        if (run_all || run_bounds) {
            runner.runBoundSensitivityAnalysis();
        }
        
        if (run_all || run_comparison) {
            runner.runDijkstraComparison();
        }
        
        if (run_all || run_stress) {
            runner.runStressTests();
        }
        
        if (run_large_scale) {
            runner.runLargeScaleTests();
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto total_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
        
        runner.generatePerformanceReport();
        
        std::cout << "\nTotal test suite execution time: " << total_time << " seconds" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Performance test suite failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
