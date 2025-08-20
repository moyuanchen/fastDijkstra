#include "Debug.h"
#include <sstream>
#include <cstring>

// Global debug flag
bool g_debug_enabled = false;

std::string vectorToString(const std::vector<int>& vec) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << vec[i];
        if (i >= 10) { // Limit output for large vectors
            oss << ", ... (" << (vec.size() - i - 1) << " more)";
            break;
        }
    }
    oss << "]";
    return oss.str();
}

std::string vectorToString(const std::vector<double>& vec) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << vec[i];
        if (i >= 10) { // Limit output for large vectors
            oss << ", ... (" << (vec.size() - i - 1) << " more)";
            break;
        }
    }
    oss << "]";
    return oss.str();
}

std::string setToString(const std::unordered_set<int>& set) {
    std::ostringstream oss;
    oss << "{";
    size_t count = 0;
    for (const auto& item : set) {
        if (count > 0) oss << ", ";
        oss << item;
        count++;
        if (count >= 10) { // Limit output for large sets
            oss << ", ... (" << (set.size() - count) << " more)";
            break;
        }
    }
    oss << "}";
    return oss.str();
}

void initializeDebug(int argc, char* argv[]) {
    // Check for debug flag in command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            g_debug_enabled = true;
            std::cout << "[DEBUG] Debug mode enabled via command line" << std::endl;
            break;
        }
    }

    // Also check environment variable
    const char* debug_env = std::getenv("DEBUG_BMSSP");
    if (debug_env && (strcmp(debug_env, "1") == 0 || strcmp(debug_env, "true") == 0)) {
        g_debug_enabled = true;
        std::cout << "[DEBUG] Debug mode enabled via environment variable" << std::endl;
    }
}
