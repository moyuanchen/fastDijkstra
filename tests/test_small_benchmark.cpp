#include "BMSSPTestFramework.h"
#include "Debug.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <numeric>

/**
 * Small Scale BMSSP vs Dijkstra Test
 * Quick test to verify the benchmark framework works correctly
 * Tests smaller graphs (1000 nodes) with different edge densities
 */

struct BenchmarkResult {
    int trial;
    int vertices;
    int edges;
    double edge_density_ratio;
    double bmssp_time_ms;
    double dijkstra_time_ms;
    double speedup_ratio;
    bool bmssp_success;
    bool dijkstra_success;
    bool correctness_verified;
    int bmssp_completed_vertices;
    double bmssp_new_bound;
    std::string error_message;
};

int main(int argc, char* argv[]) {
    std::cout << "=== SMALL SCALE BMSSP vs DIJKSTRA TEST ===" << std::endl;
    std::cout << "Quick verification of benchmark framework" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Initialize debug system
    initializeDebug(argc, argv);
    DEBUG_PRINT("Starting small scale benchmark test");
    
    BMSSPTestFramework framework;
    const int vertices = 1000;
    const int num_trials = 3;
    
    std::vector<std::pair<double, std::string>> densities = {
        {1.5, "1.5x"},
        {2.0, "2.0x"}
    };
    
    std::vector<BenchmarkResult> results;
    
    for (auto [density_ratio, density_name] : densities) {
        int edges = static_cast<int>(vertices * density_ratio);
        
        std::cout << "\nTesting " << vertices << " vertices, " << edges << " edges (" 
                  << density_name << " density)" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        for (int trial = 1; trial <= num_trials; ++trial) {
            std::cout << "Trial " << trial << ": ";
            
            try {
                // Generate test case
                TestParameters params;
                params.num_vertices = vertices;
                params.num_edges = edges;
                params.graph_type = GraphType::RANDOM_SPARSE;
                params.weight_dist = WeightDistribution::UNIFORM;
                params.source_method = SourceGenMethod::SINGLE_SOURCE;
                params.source_count = 1;
                params.bound_type = BoundType::INFINITE;
                params.k_param = static_cast<int>(std::sqrt(vertices));
                params.t_param = 3;
                params.test_name = "Small test trial " + std::to_string(trial);
                params.ensure_connectivity = true;
                params.is_directed = true;
                
                auto test_case = framework.generateTestCase(params);
                
                // Test BMSSP
                auto bmssp_start = std::chrono::high_resolution_clock::now();
                auto bmssp_output = framework.executeBMSSP(test_case);
                auto bmssp_end = std::chrono::high_resolution_clock::now();
                
                double bmssp_time = std::chrono::duration<double, std::milli>(bmssp_end - bmssp_start).count();
                
                if (!bmssp_output.execution_success) {
                    std::cout << "BMSSP failed - " << bmssp_output.error_message << std::endl;
                    continue;
                }
                
                // Test Dijkstra
                auto dijkstra_start = std::chrono::high_resolution_clock::now();
                auto dijkstra_distances = framework.runReferenceDijkstra(test_case.graph, test_case.sources);
                auto dijkstra_end = std::chrono::high_resolution_clock::now();
                
                double dijkstra_time = std::chrono::duration<double, std::milli>(dijkstra_end - dijkstra_start).count();
                
                // Verify correctness
                auto verification = framework.verifyCorrectness(test_case, bmssp_output);
                bool distances_match = verification.distances_correct;
                bool bound_correct = (bmssp_output.new_bound <= 1.0 + 1e-9);
                bool correctness_verified = distances_match && bound_correct;
                
                double speedup = dijkstra_time / bmssp_time;
                
                std::cout << std::fixed << std::setprecision(2)
                          << "BMSSP=" << bmssp_time << "ms, "
                          << "Dijkstra=" << dijkstra_time << "ms, "
                          << "Speedup=" << speedup << "x, "
                          << "Correct=" << (correctness_verified ? "✓" : "✗") << std::endl;
                
                BenchmarkResult result;
                result.trial = trial;
                result.vertices = vertices;
                result.edges = edges;
                result.edge_density_ratio = density_ratio;
                result.bmssp_time_ms = bmssp_time;
                result.dijkstra_time_ms = dijkstra_time;
                result.speedup_ratio = speedup;
                result.bmssp_success = true;
                result.dijkstra_success = true;
                result.correctness_verified = correctness_verified;
                result.bmssp_completed_vertices = bmssp_output.completed_vertices.size();
                result.bmssp_new_bound = bmssp_output.new_bound;
                
                results.push_back(result);
                
            } catch (const std::exception& e) {
                std::cout << "Exception: " << e.what() << std::endl;
            }
        }
    }
    
    // Summary
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "SUMMARY" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    int total_tests = results.size();
    int correct_tests = 0;
    double total_speedup = 0.0;
    
    for (const auto& result : results) {
        if (result.correctness_verified) correct_tests++;
        total_speedup += result.speedup_ratio;
    }
    
    std::cout << "Total tests: " << total_tests << std::endl;
    std::cout << "Correct: " << correct_tests << " (" 
              << std::fixed << std::setprecision(1) 
              << (100.0 * correct_tests / total_tests) << "%)" << std::endl;
    
    if (total_tests > 0) {
        std::cout << "Average speedup: " << std::fixed << std::setprecision(2) 
                  << (total_speedup / total_tests) << "x" << std::endl;
    }
    
    if (correct_tests == total_tests) {
        std::cout << "\n✅ Small scale test PASSED! Ready for large scale benchmark." << std::endl;
        return 0;
    } else {
        std::cout << "\n❌ Some tests failed. Check the implementation." << std::endl;
        return 1;
    }
}
