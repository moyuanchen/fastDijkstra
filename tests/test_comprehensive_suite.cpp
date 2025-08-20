#include "BMSSPTestFramework.h"
#include "Debug.h"
#include <iostream>
#include <iomanip>
#include <chrono>

/**
 * Comprehensive Test Suite
 * Uses the BMSSPTestFramework to run systematic tests covering:
 * - Different graph sizes and structures
 * - Various edge weight distributions
 * - Multiple source set configurations
 * - Different bound parameter types
 * - Correctness verification against reference implementation
 */

class ComprehensiveTestRunner {
private:
    BMSSPTestFramework framework;
    int total_tests;
    int passed_tests;
    
public:
    ComprehensiveTestRunner() : total_tests(0), passed_tests(0) {}
    
    void runGraphSizeTests() {
        std::cout << "\n1. GRAPH SIZE VARIATIONS" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        // Small graphs
        runTestSeries("Small graphs", {5, 10, 20, 50}, GraphType::RANDOM_SPARSE, 
                     WeightDistribution::UNIT_WEIGHTS, SourceGenMethod::SINGLE_SOURCE);
        
        // Medium graphs
        runTestSeries("Medium graphs", {100, 200}, GraphType::RANDOM_SPARSE, 
                     WeightDistribution::UNIFORM, SourceGenMethod::RANDOM);
        
        // Connected graphs test series - demonstrate the new functionality
        runConnectedTestSeries("Connected graphs", {10, 25, 50}, 
                              WeightDistribution::UNIFORM, SourceGenMethod::RANDOM);
    }
    
    void runGraphStructureTests() {
        std::cout << "\n2. GRAPH STRUCTURE TYPES" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        std::vector<GraphType> structures = {
            GraphType::TREE, GraphType::CYCLE, GraphType::STAR, 
            GraphType::GRID_2D, GraphType::COMPLETE
        };
        
        for (auto structure : structures) {
            std::string name = getGraphTypeName(structure);
            runSingleStructureTest(name, 20, structure);
        }
    }
    
    void runWeightDistributionTests() {
        std::cout << "\n3. EDGE WEIGHT DISTRIBUTIONS" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        std::vector<WeightDistribution> distributions = {
            WeightDistribution::UNIT_WEIGHTS,
            WeightDistribution::UNIFORM,
            WeightDistribution::INTEGER_SMALL,
            WeightDistribution::BINARY_WEIGHTS,
            WeightDistribution::EXPONENTIAL
        };
        
        for (auto dist : distributions) {
            std::string name = getWeightDistName(dist);
            runWeightDistTest(name, dist);
        }
    }
    
    void runSourceSetTests() {
        std::cout << "\n4. SOURCE SET VARIATIONS" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        std::vector<SourceGenMethod> methods = {
            SourceGenMethod::SINGLE_SOURCE,
            SourceGenMethod::RANDOM,
            SourceGenMethod::CLUSTERED,
            SourceGenMethod::SPREAD
        };
        
        for (auto method : methods) {
            std::string name = getSourceMethodName(method);
            runSourceSetTest(name, method);
        }
    }
    
    void runBoundParameterTests() {
        std::cout << "\n5. BOUND PARAMETER TESTING" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        std::vector<BoundType> bounds = {
            BoundType::TIGHT,
            BoundType::LOOSE,
            BoundType::OPTIMAL,
            BoundType::VERY_SMALL
        };
        
        for (auto bound : bounds) {
            std::string name = getBoundTypeName(bound);
            runBoundTest(name, bound);
        }
    }
    
    void runConnectivityTests() {
        std::cout << "\n6. CONNECTIVITY GUARANTEE TESTING" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        // Test directed strongly connected graphs
        runConnectivityTest("Directed Strongly Connected", true, true);
        
        // Test undirected connected graphs  
        runConnectivityTest("Undirected Connected", false, true);
        
        // Test various sizes with connectivity guarantee
        std::vector<int> sizes = {10, 25, 50};
        for (int size : sizes) {
            runConnectivitySizeTest("Connected Graph Size", size);
        }
    }
    
    void runCorrectnessVerification() {
        std::cout << "\n7. CORRECTNESS VERIFICATION" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        auto test_cases = framework.generateCorrectnessTests();
        
        for (size_t i = 0; i < test_cases.size(); i++) {
            std::cout << "Correctness test " << (i + 1) << ": " << test_cases[i].description << std::endl;
            
            auto output = framework.executeBMSSP(test_cases[i]);
            auto verification = framework.verifyCorrectness(test_cases[i], output);
            
            total_tests++;
            if (verification.distances_correct && verification.completeness_verified && 
                verification.bound_satisfaction) {
                passed_tests++;
                std::cout << "  ✓ PASSED" << std::endl;
            } else {
                std::cout << "  ✗ FAILED" << std::endl;
                for (const auto& error : verification.error_messages) {
                    std::cout << "    Error: " << error << std::endl;
                }
            }
        }
    }
    
    void printFinalSummary() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "COMPREHENSIVE TEST SUMMARY" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "Total tests run: " << total_tests << std::endl;
        std::cout << "Tests passed: " << passed_tests << std::endl;
        std::cout << "Tests failed: " << (total_tests - passed_tests) << std::endl;
        std::cout << "Success rate: " << std::fixed << std::setprecision(1) 
                  << (100.0 * passed_tests / std::max(1, total_tests)) << "%" << std::endl;
        
        if (passed_tests == total_tests) {
            std::cout << "\n🎉 ALL TESTS PASSED! Algorithm implementation verified." << std::endl;
        } else {
            std::cout << "\n⚠️  Some tests failed. Review implementation." << std::endl;
        }
    }
    
private:
    void runTestSeries(const std::string& series_name, 
                      const std::vector<int>& sizes,
                      GraphType graph_type,
                      WeightDistribution weight_dist,
                      SourceGenMethod source_method) {
        
        std::cout << series_name << ":" << std::endl;
        for (int size : sizes) {
            TestParameters params;
            params.num_vertices = size;
            params.num_edges = std::min(size * 3, size * (size - 1) / 4);
            params.graph_type = graph_type;
            params.weight_dist = weight_dist;
            params.source_method = source_method;
            params.source_count = std::max(1, size / 10);
            params.bound_type = BoundType::LOOSE;
            params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
            params.t_param = 2;
            params.test_name = series_name + " n=" + std::to_string(size);
            
            runSingleTest(params);
        }
    }
    
    void runConnectedTestSeries(const std::string& series_name,
                               const std::vector<int>& sizes,
                               WeightDistribution weight_dist,
                               SourceGenMethod source_method) {
        
        std::cout << series_name << " (guaranteed connectivity):" << std::endl;
        for (int size : sizes) {
            TestParameters params;
            params.num_vertices = size;
            params.num_edges = std::max(size * 2, size + 10);  // Ensure enough edges for connectivity
            params.graph_type = GraphType::RANDOM_SPARSE;  // Will be overridden by ensure_connectivity
            params.weight_dist = weight_dist;
            params.source_method = source_method;
            params.source_count = std::max(1, size / 10);
            params.bound_type = BoundType::LOOSE;
            params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
            params.t_param = 2;
            params.test_name = series_name + " n=" + std::to_string(size) + " (connected)";
            params.ensure_connectivity = true;  // Use the new connectivity feature
            params.is_directed = true;
            
            runSingleTest(params);
        }
    }
    
    void runSingleStructureTest(const std::string& name, int size, GraphType structure) {
        TestParameters params;
        params.num_vertices = size;
        params.num_edges = (structure == GraphType::COMPLETE) ? size * (size - 1) / 2 : size * 2;
        params.graph_type = structure;
        params.weight_dist = WeightDistribution::UNIT_WEIGHTS;
        params.source_method = SourceGenMethod::SINGLE_SOURCE;
        params.source_count = 1;
        params.bound_type = BoundType::LOOSE;
        params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
        params.t_param = 2;
        params.test_name = name + " structure test";
        
        runSingleTest(params);
    }
    
    void runWeightDistTest(const std::string& name, WeightDistribution dist) {
        TestParameters params;
        params.num_vertices = 30;
        params.num_edges = 60;
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = dist;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = 3;
        params.bound_type = BoundType::LOOSE;
        params.k_param = 3;
        params.t_param = 2;
        params.test_name = name + " weight distribution test";
        
        runSingleTest(params);
    }
    
    void runSourceSetTest(const std::string& name, SourceGenMethod method) {
        TestParameters params;
        params.num_vertices = 25;
        params.num_edges = 50;
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = method;
        params.source_count = (method == SourceGenMethod::SINGLE_SOURCE) ? 1 : 4;
        params.bound_type = BoundType::LOOSE;
        params.k_param = 3;
        params.t_param = 2;
        params.test_name = name + " source method test";
        
        runSingleTest(params);
    }
    
    void runBoundTest(const std::string& name, BoundType bound_type) {
        TestParameters params;
        params.num_vertices = 20;
        params.num_edges = 40;
        params.graph_type = GraphType::RANDOM_SPARSE;
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = 2;
        params.bound_type = bound_type;
        params.k_param = 3;
        params.t_param = 2;
        params.test_name = name + " bound test";
        
        runSingleTest(params);
    }
    
    void runConnectivityTest(const std::string& name, bool is_directed, bool ensure_connectivity) {
        TestParameters params;
        params.num_vertices = 20;
        params.num_edges = 35;  // Ensure enough edges for connectivity plus extras
        params.graph_type = GraphType::RANDOM_SPARSE;  // Will be overridden by ensure_connectivity
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = 3;
        params.bound_type = BoundType::LOOSE;
        params.k_param = 3;
        params.t_param = 2;
        params.test_name = name + " connectivity test";
        params.ensure_connectivity = ensure_connectivity;
        params.is_directed = is_directed;
        
        runSingleTest(params);
    }
    
    void runConnectivitySizeTest(const std::string& name, int size) {
        TestParameters params;
        params.num_vertices = size;
        params.num_edges = std::max(size * 2, size + 10);  // Ensure enough edges
        params.graph_type = GraphType::RANDOM_SPARSE;  // Will be overridden by ensure_connectivity
        params.weight_dist = WeightDistribution::UNIFORM;
        params.source_method = SourceGenMethod::RANDOM;
        params.source_count = std::max(1, size / 8);
        params.bound_type = BoundType::LOOSE;
        params.k_param = std::max(1, static_cast<int>(std::sqrt(size)));
        params.t_param = 2;
        params.test_name = name + " n=" + std::to_string(size);
        params.ensure_connectivity = true;
        params.is_directed = true;  // Test directed graphs by default
        
        runSingleTest(params);
    }
    
    void runSingleTest(const TestParameters& params) {
        total_tests++;
        
        DEBUG_PRINT("Starting test: " << params.test_name);
        DEBUG_PRINT("Parameters: vertices=" << params.num_vertices << ", edges=" << params.num_edges << ", sources=" << params.source_count);
        
        try {
            auto test_case = framework.generateTestCase(params);
            DEBUG_PRINT("Generated test case successfully");
            
            auto output = framework.executeBMSSP(test_case);
            DEBUG_PRINT("Executed BMSSP, success=" << output.execution_success);

            if (output.execution_success) {
                auto verification = framework.verifyCorrectness(test_case, output);
                
                if (verification.distances_correct && verification.completeness_verified && 
                    verification.bound_satisfaction) {
                    passed_tests++;
                    std::cout << "  ✓ " << params.test_name << " (time: " 
                              << std::fixed << std::setprecision(1) << output.execution_time_ms << "ms)" << std::endl;
                } else {
                    DEBUG_PRINT("Verification failed for test: " << params.test_name);
                    std::cout << "  ✗ " << params.test_name << " - Verification failed" << std::endl;
                }
            } else {
                DEBUG_PRINT("Execution failed for test: " << params.test_name << ", error: " << output.error_message);
                std::cout << "  ✗ " << params.test_name << " - Execution failed: " << output.error_message << std::endl;
            }
            
        } catch (const std::exception& e) {
            DEBUG_PRINT("Exception in test: " << params.test_name << ", exception: " << e.what());
            std::cout << "  ✗ " << params.test_name << " - Exception: " << e.what() << std::endl;
        }
    }
    
    std::string getGraphTypeName(GraphType type) {
        switch (type) {
            case GraphType::RANDOM_SPARSE: return "Random Sparse";
            case GraphType::TREE: return "Tree";
            case GraphType::CYCLE: return "Cycle";
            case GraphType::GRID_2D: return "Grid 2D";
            case GraphType::STAR: return "Star";
            case GraphType::COMPLETE: return "Complete";
            default: return "Unknown";
        }
    }
    
    std::string getWeightDistName(WeightDistribution dist) {
        switch (dist) {
            case WeightDistribution::UNIFORM: return "Uniform";
            case WeightDistribution::EXPONENTIAL: return "Exponential";
            case WeightDistribution::INTEGER_SMALL: return "Small Integer";
            case WeightDistribution::INTEGER_LARGE: return "Large Integer";
            case WeightDistribution::UNIT_WEIGHTS: return "Unit Weights";
            case WeightDistribution::BINARY_WEIGHTS: return "Binary Weights";
            case WeightDistribution::POWER_OF_TWO: return "Power of Two";
            default: return "Unknown";
        }
    }
    
    std::string getSourceMethodName(SourceGenMethod method) {
        switch (method) {
            case SourceGenMethod::RANDOM: return "Random";
            case SourceGenMethod::CLUSTERED: return "Clustered";
            case SourceGenMethod::SPREAD: return "Spread";
            case SourceGenMethod::SINGLE_SOURCE: return "Single Source";
            default: return "Unknown";
        }
    }
    
    std::string getBoundTypeName(BoundType type) {
        switch (type) {
            case BoundType::ZERO: return "Zero";
            case BoundType::VERY_SMALL: return "Very Small";
            case BoundType::OPTIMAL: return "Optimal";
            case BoundType::TIGHT: return "Tight";
            case BoundType::LOOSE: return "Loose";
            case BoundType::INFINITE: return "Infinite";
            default: return "Unknown";
        }
    }
};

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS]\n"
              << "\nOptions:\n"
              << "  --size-tests      Run graph size variation tests\n"
              << "  --structure-tests Run graph structure tests\n"
              << "  --weight-tests    Run weight distribution tests\n"
              << "  --source-tests    Run source set variation tests\n"
              << "  --bound-tests     Run bound parameter tests\n"
              << "  --connectivity    Run connectivity guarantee tests\n"
              << "  --correctness     Run correctness verification\n"
              << "  --all             Run all test suites (default)\n"
              << "  --debug, -d       Enable debug output\n"
              << "  --help            Show this help message\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "=== COMPREHENSIVE BMSSP TEST SUITE ===" << std::endl;
    std::cout << "Based on the comprehensive test plan" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Initialize debug system
    initializeDebug(argc, argv);
    DEBUG_PRINT("Comprehensive test suite starting");
    
    ComprehensiveTestRunner runner;
    
    // Parse command line arguments
    bool run_all = true;
    bool run_size = false, run_structure = false, run_weight = false;
    bool run_source = false, run_bound = false, run_connectivity = false, run_correctness = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--debug" || arg == "-d") {
            // Already handled in initializeDebug
            continue;
        } else if (arg == "--size-tests") {
            run_size = true; run_all = false;
        } else if (arg == "--structure-tests") {
            run_structure = true; run_all = false;
        } else if (arg == "--weight-tests") {
            run_weight = true; run_all = false;
        } else if (arg == "--source-tests") {
            run_source = true; run_all = false;
        } else if (arg == "--bound-tests") {
            run_bound = true; run_all = false;
        } else if (arg == "--connectivity") {
            run_connectivity = true; run_all = false;
        } else if (arg == "--correctness") {
            run_correctness = true; run_all = false;
        } else if (arg == "--all") {
            run_all = true;
        } else {
            std::cout << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    try {
        DEBUG_PRINT("Starting test execution phase");
        
        if (run_all || run_size) {
            DEBUG_PRINT("Running graph size tests");
            runner.runGraphSizeTests();
        }
        
        if (run_all || run_structure) {
            DEBUG_PRINT("Running graph structure tests");
            runner.runGraphStructureTests();
        }
        
        if (run_all || run_weight) {
            DEBUG_PRINT("Running weight distribution tests");
            runner.runWeightDistributionTests();
        }
        
        if (run_all || run_source) {
            DEBUG_PRINT("Running source set tests");
            runner.runSourceSetTests();
        }
        
        if (run_all || run_bound) {
            DEBUG_PRINT("Running bound parameter tests");
            runner.runBoundParameterTests();
        }
        
        if (run_all || run_connectivity) {
            DEBUG_PRINT("Running connectivity tests");
            runner.runConnectivityTests();
        }
        
        if (run_all || run_correctness) {
            DEBUG_PRINT("Running correctness verification");
            runner.runCorrectnessVerification();
        }
        
        runner.printFinalSummary();
        
    } catch (const std::exception& e) {
        DEBUG_PRINT("Exception in main: " << e.what());
        std::cerr << "\n❌ Test suite failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    DEBUG_PRINT("Comprehensive test suite completed");
    return 0;
}
