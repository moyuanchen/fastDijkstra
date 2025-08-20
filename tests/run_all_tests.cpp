#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

/**
 * Master Test Runner
 * Centralized test execution for all BMSSP test suites:
 * 1. Core Functionality Tests
 * 2. Comprehensive Test Suite  
 * 3. Edge Cases Tests
 * 4. Performance Tests
 */

void printUsage(const char* program_name) {
    std::cout << "BMSSP Master Test Runner\n"
              << "========================\n\n"
              << "Usage: " << program_name << " [OPTIONS]\n\n"
              << "Test Suites:\n"
              << "  --core            Run core functionality tests\n"
              << "  --comprehensive   Run comprehensive algorithm tests\n"
              << "  --edge-cases      Run edge cases and error handling tests\n"
              << "  --performance     Run performance and scalability tests\n"
              << "  --large-scale     Run large scale testing (up to 10K vertices)\n"
              << "  --all             Run all test suites (default)\n\n"
              << "Additional Options:\n"
              << "  --quick           Run quick subset of tests\n"
              << "  --detailed        Run detailed/verbose tests\n"
              << "  --help            Show this help message\n\n"
              << "Examples:\n"
              << "  " << program_name << "                    # Run all tests\n"
              << "  " << program_name << " --core             # Run only core tests\n"
              << "  " << program_name << " --comprehensive    # Run comprehensive suite\n"
              << "  " << program_name << " --performance --detailed  # Detailed performance tests\n"
              << std::endl;
}

int runTestSuite(const std::string& test_name, const std::string& executable, 
                const std::vector<std::string>& args = {}) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "RUNNING: " << test_name << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Build command
    std::string command = "./" + executable;
    for (const auto& arg : args) {
        command += " " + arg;
    }
    
    std::cout << "Executing: " << command << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    int result = std::system(command.c_str());
    
    if (result == 0) {
        std::cout << "\n✅ " << test_name << " PASSED" << std::endl;
    } else {
        std::cout << "\n❌ " << test_name << " FAILED (exit code: " << result << ")" << std::endl;
    }
    
    return result;
}

void printSummary(const std::vector<std::pair<std::string, int>>& results) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "FINAL TEST SUMMARY" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    int total_suites = results.size();
    int passed_suites = 0;
    
    for (const auto& result : results) {
        std::string status = (result.second == 0) ? "✅ PASSED" : "❌ FAILED";
        std::cout << std::left << std::setw(30) << result.first << " : " << status << std::endl;
        if (result.second == 0) passed_suites++;
    }
    
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "Test Suites: " << passed_suites << "/" << total_suites << " passed" << std::endl;
    
    if (passed_suites == total_suites) {
        std::cout << "\n🎉 ALL TEST SUITES PASSED! 🎉" << std::endl;
        std::cout << "✓ Core functionality verified" << std::endl;
        std::cout << "✓ Comprehensive testing completed" << std::endl;
        std::cout << "✓ Edge cases handled correctly" << std::endl;
        std::cout << "✓ Performance characteristics analyzed" << std::endl;
        std::cout << "✓ Large scale scalability verified" << std::endl;
        std::cout << "\n🚀 BMSSP implementation is ready for use!" << std::endl;
    } else {
        std::cout << "\n⚠️  Some test suites failed. Please review the output above." << std::endl;
        std::cout << "💡 Tip: Run individual test suites to debug specific issues." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "BMSSP Algorithm Test Suite" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Comprehensive testing of Bounded Multi-Source Shortest Path implementation" << std::endl;
    
    // Parse command line arguments
    bool run_all = true;
    bool run_core = false, run_comprehensive = false, run_edge_cases = false, run_performance = false, run_large_scale = false;
    bool quick_mode = false, detailed_mode = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--core") {
            run_core = true; run_all = false;
        } else if (arg == "--comprehensive") {
            run_comprehensive = true; run_all = false;
        } else if (arg == "--edge-cases") {
            run_edge_cases = true; run_all = false;
        } else if (arg == "--performance") {
            run_performance = true; run_all = false;
        } else if (arg == "--large-scale") {
            run_large_scale = true; run_all = false;
        } else if (arg == "--all") {
            run_all = true;
        } else if (arg == "--quick") {
            quick_mode = true;
        } else if (arg == "--detailed") {
            detailed_mode = true;
        } else {
            std::cout << "Unknown option: " << arg << std::endl;
            std::cout << "Use --help for usage information." << std::endl;
            return 1;
        }
    }
    
    std::vector<std::pair<std::string, int>> results;
    
    // Run selected test suites
    if (run_all || run_core) {
        int result = runTestSuite("Core Functionality Tests", "test_core_functionality");
        results.emplace_back("Core Functionality", result);
    }
    
    if (run_all || run_comprehensive) {
        std::vector<std::string> args;
        if (quick_mode) {
            args.push_back("--size-tests");
        } else if (detailed_mode) {
            args.push_back("--all");
        } else {
            args.push_back("--correctness");
        }
        
        int result = runTestSuite("Comprehensive Algorithm Tests", "test_comprehensive_suite", args);
        results.emplace_back("Comprehensive Tests", result);
    }
    
    if (run_all || run_edge_cases) {
        int result = runTestSuite("Edge Cases and Error Handling", "test_edge_cases");
        results.emplace_back("Edge Cases", result);
    }
    
    if (run_all || run_performance) {
        std::vector<std::string> args;
        if (quick_mode) {
            args.push_back("--scalability");
        } else if (detailed_mode) {
            args.push_back("--all");
        } else {
            args.push_back("--scalability");
            args.push_back("--comparison");
        }
        
        int result = runTestSuite("Performance and Scalability Tests", "test_performance", args);
        results.emplace_back("Performance Tests", result);
    }
    
    if (run_all || run_large_scale) {
        int result = runTestSuite("Large Scale Testing", "test_large_scale");
        results.emplace_back("Large Scale Tests", result);
    }
    
    // Print final summary
    printSummary(results);
    
    // Return overall status
    bool all_passed = true;
    for (const auto& result : results) {
        if (result.second != 0) {
            all_passed = false;
            break;
        }
    }
    
    return all_passed ? 0 : 1;
}
