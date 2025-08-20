#include "BMSSPTestFramework.h"
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
    
    void runCorrectnessVerification() {
        std::cout << "\n6. CORRECTNESS VERIFICATION" << std::endl;
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
    
    void runSingleTest(const TestParameters& params) {
        total_tests++;
        
        try {
            auto test_case = framework.generateTestCase(params);
            auto output = framework.executeBMSSP(test_case);
            
            if (output.execution_success) {
                auto verification = framework.verifyCorrectness(test_case, output);
                
                if (verification.distances_correct && verification.completeness_verified && 
                    verification.bound_satisfaction) {
                    passed_tests++;
                    std::cout << "  ✓ " << params.test_name << " (time: " 
                              << std::fixed << std::setprecision(1) << output.execution_time_ms << "ms)" << std::endl;
                } else {
                    std::cout << "  ✗ " << params.test_name << " - Verification failed" << std::endl;
                }
            } else {
                std::cout << "  ✗ " << params.test_name << " - Execution failed: " << output.error_message << std::endl;
            }
            
        } catch (const std::exception& e) {
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
              << "  --correctness     Run correctness verification\n"
              << "  --all             Run all test suites (default)\n"
              << "  --help            Show this help message\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "=== COMPREHENSIVE BMSSP TEST SUITE ===" << std::endl;
    std::cout << "Based on the comprehensive test plan" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    ComprehensiveTestRunner runner;
    
    // Parse command line arguments
    bool run_all = true;
    bool run_size = false, run_structure = false, run_weight = false;
    bool run_source = false, run_bound = false, run_correctness = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
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
        if (run_all || run_size) {
            runner.runGraphSizeTests();
        }
        
        if (run_all || run_structure) {
            runner.runGraphStructureTests();
        }
        
        if (run_all || run_weight) {
            runner.runWeightDistributionTests();
        }
        
        if (run_all || run_source) {
            runner.runSourceSetTests();
        }
        
        if (run_all || run_bound) {
            runner.runBoundParameterTests();
        }
        
        if (run_all || run_correctness) {
            runner.runCorrectnessVerification();
        }
        
        runner.printFinalSummary();
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test suite failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
