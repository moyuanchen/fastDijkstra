#!/bin/bash

# BMSSP Test Suite Build and Run Script
# =====================================
# This script builds the consolidated test suite and provides easy execution options

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to show usage
show_usage() {
    echo "BMSSP Test Suite Build and Run Script"
    echo "====================================="
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Build Options:"
    echo "  --build-only     Build test suite without running"
    echo "  --clean-build    Clean and rebuild from scratch"
    echo ""
    echo "Test Options:"
    echo "  --run-all        Build and run all tests (default)"
    echo "  --core           Run only core functionality tests"
    echo "  --comprehensive  Run comprehensive test suite"
    echo "  --edge-cases     Run edge cases tests"
    echo "  --performance    Run performance tests"
    echo "  --quick          Run quick subset of tests"
    echo "  --detailed       Run detailed/verbose tests"
    echo ""
    echo "Other Options:"
    echo "  --help           Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                          # Build and run all tests"
    echo "  $0 --core                   # Build and run core tests only"
    echo "  $0 --clean-build            # Clean build and run all tests"
    echo "  $0 --performance --detailed # Detailed performance analysis"
}

# Default values
BUILD_ONLY=false
CLEAN_BUILD=false
RUN_TESTS=true
TEST_MODE="all"
TEST_ARGS=""

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --help|-h)
            show_usage
            exit 0
            ;;
        --build-only)
            BUILD_ONLY=true
            RUN_TESTS=false
            shift
            ;;
        --clean-build)
            CLEAN_BUILD=true
            shift
            ;;
        --run-all)
            TEST_MODE="all"
            shift
            ;;
        --core)
            TEST_MODE="core"
            TEST_ARGS="--core"
            shift
            ;;
        --comprehensive)
            TEST_MODE="comprehensive"
            TEST_ARGS="--comprehensive"
            shift
            ;;
        --edge-cases)
            TEST_MODE="edge-cases"
            TEST_ARGS="--edge-cases"
            shift
            ;;
        --performance)
            TEST_MODE="performance"
            TEST_ARGS="--performance"
            shift
            ;;
        --quick)
            TEST_ARGS="$TEST_ARGS --quick"
            shift
            ;;
        --detailed)
            TEST_ARGS="$TEST_ARGS --detailed"
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use --help for usage information."
            exit 1
            ;;
    esac
done

print_status "Starting BMSSP Test Suite Build Process"

# Check if we're in the right directory
if [[ ! -f "CMakeLists.txt" ]]; then
    print_error "CMakeLists.txt not found. Please run this script from the project root directory."
    exit 1
fi

# Clean build if requested
if [[ "$CLEAN_BUILD" == true ]]; then
    print_status "Cleaning previous build..."
    if [[ -d "build" ]]; then
        rm -rf build
        print_success "Build directory cleaned"
    fi
fi

# Create build directory
if [[ ! -d "build" ]]; then
    print_status "Creating build directory..."
    mkdir build
fi

cd build

# Configure build
print_status "Configuring build..."
CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Release"

if ! cmake .. $CMAKE_ARGS; then
    print_error "CMake configuration failed"
    exit 1
fi

print_success "Build configured successfully"

# Build the project
print_status "Building test suite..."
if ! make -j$(nproc 2>/dev/null || echo 4); then
    print_error "Build failed"
    exit 1
fi

print_success "Build completed successfully"

# List built test executables
print_status "Available test executables:"
for test_exe in test_core_functionality test_comprehensive_suite test_edge_cases test_performance run_all_tests; do
    if [[ -f "$test_exe" ]]; then
        echo "  ✓ $test_exe"
    else
        print_warning "  ✗ $test_exe (not built)"
    fi
done

# Exit if build-only was requested
if [[ "$BUILD_ONLY" == true ]]; then
    print_success "Build-only completed. Test executables ready in build directory."
    exit 0
fi

# Run tests if requested
if [[ "$RUN_TESTS" == true ]]; then
    echo ""
    print_status "Running tests in mode: $TEST_MODE"
    
    # Check if master test runner exists
    if [[ -f "run_all_tests" ]]; then
        print_status "Using master test runner..."
        if ./run_all_tests $TEST_ARGS; then
            print_success "All tests completed successfully!"
        else
            print_error "Some tests failed. Check output above for details."
            exit 1
        fi
    else
        print_error "Master test runner not found. Build may have failed."
        exit 1
    fi
fi

print_success "BMSSP Test Suite execution completed!"

# Show next steps
echo ""
echo "Next steps:"
echo "  • Review test results above"
echo "  • For individual test execution, cd to build directory"
echo "  • Run specific tests: ./test_core_functionality, ./test_performance, etc."
echo "  • For performance analysis, check performance_results.csv (if generated)"
