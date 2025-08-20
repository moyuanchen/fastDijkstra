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
 * BMSSP vs Dijkstra Benchmark Test
 * Comprehensive performance comparison between BMSSP and Dijkstra algorithms
 * Tests 100,000 nodes with edge densities of 1.5x, 2x, and 3x nodes
 * Runs 10 trials for each configuration and saves results to CSV
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
    size_t memory_usage_kb;
    std::string error_message;
};

class BMSSPDijkstraBenchmark {
private:
    BMSSPTestFramework framework;
    std::vector<BenchmarkResult> results;
    
    // Memory tracking
    size_t getCurrentMemoryUsage() {
        // Simple memory tracking - in a real implementation you might use more sophisticated methods
        // For now, return 0 as placeholder
        return 0;
    }
    
    // Calculate statistics for multiple trials
    struct TrialStatistics {
        double mean_time_ms;
        double std_dev_ms;
        double min_time_ms;
        double max_time_ms;
        double median_time_ms;
        int successful_trials;
    };
    
    TrialStatistics calculateStatistics(const std::vector<double>& times) {
        TrialStatistics stats;
        
        if (times.empty()) {
            stats = {0, 0, 0, 0, 0, 0};
            return stats;
        }
        
        stats.successful_trials = times.size();
        
        // Calculate mean
        stats.mean_time_ms = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        
        // Calculate standard deviation
        double variance = 0.0;
        for (double time : times) {
            variance += (time - stats.mean_time_ms) * (time - stats.mean_time_ms);
        }
        stats.std_dev_ms = std::sqrt(variance / times.size());
        
        // Calculate min, max, median
        std::vector<double> sorted_times = times;
        std::sort(sorted_times.begin(), sorted_times.end());
        
        stats.min_time_ms = sorted_times.front();
        stats.max_time_ms = sorted_times.back();
        stats.median_time_ms = sorted_times[sorted_times.size() / 2];
        
        return stats;
    }
    
public:
    void runBenchmark() {
        std::cout << "=== BMSSP vs DIJKSTRA BENCHMARK ===" << std::endl;
        std::cout << "Testing 100,000 nodes with different edge densities" << std::endl;
        std::cout << "100 trials per configuration" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        
        // Initialize debug system
        DEBUG_PRINT("Starting BMSSP vs Dijkstra benchmark");
        
        const int vertices = 100000;
        const int num_trials = 100;
        
        // Test different edge densities
        std::vector<std::pair<double, std::string>> densities = {
            {1.5, "1.5x"},
            {2.0, "2.0x"},
            {3.0, "3.0x"}
        };
        
        for (auto [density_ratio, density_name] : densities) {
            int edges = static_cast<int>(vertices * density_ratio);
            
            std::cout << "\n" << std::string(70, '=') << std::endl;
            std::cout << "TESTING: " << vertices << " vertices, " << edges << " edges (" 
                      << density_name << " density)" << std::endl;
            std::cout << std::string(70, '=') << std::endl;
            
            std::vector<double> bmssp_times;
            std::vector<double> dijkstra_times;
            std::vector<double> speedup_ratios;
            int correctness_verified_count = 0;
            
            for (int trial = 1; trial <= num_trials; ++trial) {
                // Show progress every 10 trials to avoid too much output
                if (trial % 10 == 1 || trial % 10 == 0) {
                    std::cout << "\n--- Trial " << trial << "/" << num_trials << " ---" << std::endl;
                } else {
                    std::cout << "Trial " << trial << "... " << std::flush;
                }
                
                BenchmarkResult result = runSingleTrial(trial, vertices, edges, density_ratio);
                results.push_back(result);
                
                // Print trial results for every 10th trial or show brief status
                if (trial % 10 == 1 || trial % 10 == 0) {
                    std::cout << "BMSSP Time: " << std::fixed << std::setprecision(2) 
                              << result.bmssp_time_ms << " ms" << std::endl;
                    std::cout << "Dijkstra Time: " << std::fixed << std::setprecision(2) 
                              << result.dijkstra_time_ms << " ms" << std::endl;
                    std::cout << "Speedup: " << std::fixed << std::setprecision(2) 
                              << result.speedup_ratio << "x" << std::endl;
                    std::cout << "Correctness: " << (result.correctness_verified ? "✓" : "✗") << std::endl;
                } else {
                    std::cout << std::fixed << std::setprecision(1) << result.speedup_ratio << "x ";
                    if (trial % 10 == 0) std::cout << std::endl;
                }
                
                if (result.bmssp_success && result.dijkstra_success) {
                    bmssp_times.push_back(result.bmssp_time_ms);
                    dijkstra_times.push_back(result.dijkstra_time_ms);
                    speedup_ratios.push_back(result.speedup_ratio);
                }
                
                if (result.correctness_verified) {
                    correctness_verified_count++;
                }
                
                if (!result.error_message.empty()) {
                    std::cout << "Error: " << result.error_message << std::endl;
                }
            }
            
            // Print summary statistics for this density
            printDensitySummary(density_name, vertices, edges, bmssp_times, dijkstra_times, 
                              speedup_ratios, correctness_verified_count, num_trials);
        }
        
        // Save results to CSV
        saveResultsToCSV();
        
        // Print overall summary
        printOverallSummary();
    }
    
private:
    BenchmarkResult runSingleTrial(int trial, int vertices, int edges, double density_ratio) {
        BenchmarkResult result;
        result.trial = trial;
        result.vertices = vertices;
        result.edges = edges;
        result.edge_density_ratio = density_ratio;
        result.bmssp_success = false;
        result.dijkstra_success = false;
        result.correctness_verified = false;
        result.memory_usage_kb = 0;
        
        try {
            // Generate test case
            TestParameters params;
            params.num_vertices = vertices;
            params.num_edges = edges;
            params.graph_type = GraphType::RANDOM_SPARSE;
            params.weight_dist = WeightDistribution::UNIFORM;
            params.source_method = SourceGenMethod::SINGLE_SOURCE;
            params.source_count = 1;
            params.bound_type = BoundType::INFINITE;  // Use B = ∞ for fair comparison
            params.k_param = static_cast<int>(std::sqrt(vertices));
            params.t_param = static_cast<int>(std::floor(std::cbrt(std::log(vertices)) * std::log(std::log(vertices))));
            params.test_name = "Benchmark trial " + std::to_string(trial);
            params.ensure_connectivity = true;
            params.is_directed = true;
            
            std::cout << "Generating test case..." << std::flush;
            auto test_case = framework.generateTestCase(params);
            std::cout << " Done. " << std::flush;
            
            // Measure memory usage
            result.memory_usage_kb = getCurrentMemoryUsage();
            
            // Test BMSSP
            std::cout << "BMSSP..." << std::flush;
            auto bmssp_start = std::chrono::high_resolution_clock::now();
            auto bmssp_output = framework.executeBMSSP(test_case);
            auto bmssp_end = std::chrono::high_resolution_clock::now();
            
            result.bmssp_time_ms = std::chrono::duration<double, std::milli>(bmssp_end - bmssp_start).count();
            result.bmssp_success = bmssp_output.execution_success;
            result.bmssp_completed_vertices = bmssp_output.completed_vertices.size();
            result.bmssp_new_bound = bmssp_output.new_bound;
            std::cout << " Done. " << std::flush;
            
            if (!bmssp_output.execution_success) {
                result.error_message = "BMSSP failed: " + bmssp_output.error_message;
                return result;
            }
            
            // Test Dijkstra
            std::cout << "Dijkstra..." << std::flush;
            auto dijkstra_start = std::chrono::high_resolution_clock::now();
            auto dijkstra_distances = framework.runReferenceDijkstra(test_case.graph, test_case.sources);
            auto dijkstra_end = std::chrono::high_resolution_clock::now();
            
            result.dijkstra_time_ms = std::chrono::duration<double, std::milli>(dijkstra_end - dijkstra_start).count();
            result.dijkstra_success = true;
            std::cout << " Done. " << std::flush;
            
            // Calculate speedup
            if (result.dijkstra_time_ms > 0) {
                result.speedup_ratio = result.dijkstra_time_ms / result.bmssp_time_ms;
            } else {
                result.speedup_ratio = 0.0;
            }
            
            // Verify correctness
            std::cout << "Verifying..." << std::flush;
            auto verification = framework.verifyCorrectness(test_case, bmssp_output);
            
            // For B = ∞, check if the algorithm finds correct shortest paths
            bool distances_match = verification.distances_correct;
            bool bound_correct = (bmssp_output.new_bound <= 1.0 + 1e-9); // For B = ∞, final bound should be ≤ 1
            
            result.correctness_verified = distances_match && bound_correct;
            std::cout << " Done" << std::endl;
            
            if (!result.correctness_verified) {
                std::stringstream ss;
                ss << "Correctness verification failed - ";
                if (!distances_match) ss << "distances don't match; ";
                if (!bound_correct) ss << "bound condition violated (" << bmssp_output.new_bound << " > 1); ";
                result.error_message = ss.str();
            }
            
        } catch (const std::exception& e) {
            result.error_message = "Exception: " + std::string(e.what());
        } catch (...) {
            result.error_message = "Unknown exception occurred";
        }
        
        return result;
    }
    
    void printDensitySummary(const std::string& density_name, int vertices, int edges,
                           const std::vector<double>& bmssp_times,
                           const std::vector<double>& dijkstra_times,
                           const std::vector<double>& speedup_ratios,
                           int correctness_count, int total_trials) {
        
        std::cout << "\n" << std::string(50, '-') << std::endl;
        std::cout << "SUMMARY FOR " << density_name << " DENSITY (" << vertices << " vertices, " << edges << " edges)" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        if (!bmssp_times.empty() && !dijkstra_times.empty()) {
            auto bmssp_stats = calculateStatistics(bmssp_times);
            auto dijkstra_stats = calculateStatistics(dijkstra_times);
            auto speedup_stats = calculateStatistics(speedup_ratios);
            
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "BMSSP    - Mean: " << bmssp_stats.mean_time_ms << "ms, "
                      << "Std: " << bmssp_stats.std_dev_ms << "ms, "
                      << "Range: [" << bmssp_stats.min_time_ms << ", " << bmssp_stats.max_time_ms << "]ms" << std::endl;
            
            std::cout << "Dijkstra - Mean: " << dijkstra_stats.mean_time_ms << "ms, "
                      << "Std: " << dijkstra_stats.std_dev_ms << "ms, "
                      << "Range: [" << dijkstra_stats.min_time_ms << ", " << dijkstra_stats.max_time_ms << "]ms" << std::endl;
            
            std::cout << "Speedup  - Mean: " << speedup_stats.mean_time_ms << "x, "
                      << "Std: " << speedup_stats.std_dev_ms << "x, "
                      << "Range: [" << speedup_stats.min_time_ms << ", " << speedup_stats.max_time_ms << "]x" << std::endl;
        }
        
        std::cout << "Correctness: " << correctness_count << "/" << total_trials 
                  << " (" << std::fixed << std::setprecision(1) 
                  << (100.0 * correctness_count / total_trials) << "%)" << std::endl;
    }
    
    void printOverallSummary() {
        std::cout << "\n" << std::string(70, '=') << std::endl;
        std::cout << "OVERALL BENCHMARK SUMMARY" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        
        int total_tests = results.size();
        int successful_tests = 0;
        int correct_tests = 0;
        
        std::vector<double> all_speedups;
        
        for (const auto& result : results) {
            if (result.bmssp_success && result.dijkstra_success) {
                successful_tests++;
                all_speedups.push_back(result.speedup_ratio);
            }
            if (result.correctness_verified) {
                correct_tests++;
            }
        }
        
        std::cout << "Total tests: " << total_tests << std::endl;
        std::cout << "Successful: " << successful_tests << " (" 
                  << std::fixed << std::setprecision(1) 
                  << (100.0 * successful_tests / total_tests) << "%)" << std::endl;
        std::cout << "Correct: " << correct_tests << " (" 
                  << std::fixed << std::setprecision(1) 
                  << (100.0 * correct_tests / total_tests) << "%)" << std::endl;
        
        if (!all_speedups.empty()) {
            auto overall_speedup_stats = calculateStatistics(all_speedups);
            std::cout << "\nOverall Speedup Statistics:" << std::endl;
            std::cout << "  Mean: " << std::fixed << std::setprecision(2) << overall_speedup_stats.mean_time_ms << "x" << std::endl;
            std::cout << "  Std Dev: " << overall_speedup_stats.std_dev_ms << "x" << std::endl;
            std::cout << "  Range: [" << overall_speedup_stats.min_time_ms << ", " << overall_speedup_stats.max_time_ms << "]x" << std::endl;
        }
        
        if (correct_tests == total_tests) {
            std::cout << "\n🎉 ALL CORRECTNESS TESTS PASSED!" << std::endl;
        } else {
            std::cout << "\n⚠️  SOME CORRECTNESS TESTS FAILED!" << std::endl;
        }
    }
    
    void saveResultsToCSV() {
        std::string filename = "/Users/moyuanchen/fastDijkstra/tests/visuals/bmssp_dijkstra_benchmark_results.csv";
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open " << filename << " for writing" << std::endl;
            return;
        }
        
        // Write CSV header
        file << "trial,vertices,edges,edge_density_ratio,bmssp_time_ms,dijkstra_time_ms,"
             << "speedup_ratio,bmssp_success,dijkstra_success,correctness_verified,"
             << "bmssp_completed_vertices,bmssp_new_bound,memory_usage_kb,error_message\n";
        
        // Write data
        for (const auto& result : results) {
            file << result.trial << ","
                 << result.vertices << ","
                 << result.edges << ","
                 << std::fixed << std::setprecision(1) << result.edge_density_ratio << ","
                 << std::fixed << std::setprecision(3) << result.bmssp_time_ms << ","
                 << std::fixed << std::setprecision(3) << result.dijkstra_time_ms << ","
                 << std::fixed << std::setprecision(3) << result.speedup_ratio << ","
                 << (result.bmssp_success ? "true" : "false") << ","
                 << (result.dijkstra_success ? "true" : "false") << ","
                 << (result.correctness_verified ? "true" : "false") << ","
                 << result.bmssp_completed_vertices << ","
                 << std::fixed << std::setprecision(6) << result.bmssp_new_bound << ","
                 << result.memory_usage_kb << ","
                 << "\"" << result.error_message << "\"\n";
        }
        
        file.close();
        std::cout << "\nResults saved to: " << filename << std::endl;
    }
};

int main(int argc, char* argv[]) {
    // Initialize debug system
    initializeDebug(argc, argv);
    
    BMSSPDijkstraBenchmark benchmark;
    
    try {
        benchmark.runBenchmark();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Benchmark failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Benchmark failed with unknown exception" << std::endl;
        return 1;
    }
}
