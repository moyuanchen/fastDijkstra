#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

// Debug control - can be controlled via compile-time flag or runtime variable
#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED 0
#endif

// Global runtime debug flag - can be toggled via command line argument
extern bool g_debug_enabled;

// Debug macros for different levels
#define DEBUG_PRINT(msg) do { \
    if (DEBUG_ENABLED || g_debug_enabled) { \
        std::cout << "[DEBUG] " << __FUNCTION__ << ":" << __LINE__ << " - " << msg << std::endl; \
    } \
} while(0)

#define DEBUG_FUNCTION_ENTRY(func_name, params) do { \
    if (DEBUG_ENABLED || g_debug_enabled) { \
        std::cout << "[DEBUG] ENTRY: " << func_name << " " << params << std::endl; \
    } \
} while(0)

#define DEBUG_FUNCTION_EXIT(func_name, result) do { \
    if (DEBUG_ENABLED || g_debug_enabled) { \
        std::cout << "[DEBUG] EXIT: " << func_name << " result=" << result << std::endl; \
    } \
} while(0)

#define DEBUG_MEMORY(msg) do { \
    if (DEBUG_ENABLED || g_debug_enabled) { \
        std::cout << "[DEBUG] MEMORY: " << __FUNCTION__ << ":" << __LINE__ << " - " << msg << std::endl; \
    } \
} while(0)

#define DEBUG_LOOP(iteration, msg) do { \
    if (DEBUG_ENABLED || g_debug_enabled) { \
        std::cout << "[DEBUG] LOOP[" << iteration << "]: " << __FUNCTION__ << " - " << msg << std::endl; \
    } \
} while(0)

#define DEBUG_DATASTRUCTURE(operation, msg) do { \
    if (DEBUG_ENABLED || g_debug_enabled) { \
        std::cout << "[DEBUG] DS_" << operation << ": " << __FUNCTION__ << " - " << msg << std::endl; \
    } \
} while(0)

#define DEBUG_POINTER_CHECK(ptr, name) do { \
    if (DEBUG_ENABLED || g_debug_enabled) { \
        if (ptr == nullptr) { \
            std::cout << "[DEBUG] WARNING: Null pointer detected for " << name << " in " << __FUNCTION__ << ":" << __LINE__ << std::endl; \
        } \
    } \
} while(0)

#define DEBUG_BOUNDS_CHECK(index, size, name) do { \
    if (DEBUG_ENABLED || g_debug_enabled) { \
        if (index < 0 || index >= size) { \
            std::cout << "[DEBUG] WARNING: Bounds violation for " << name << " index=" << index << " size=" << size << " in " << __FUNCTION__ << ":" << __LINE__ << std::endl; \
        } \
    } \
} while(0)

// Helper functions for printing complex data structures
std::string vectorToString(const std::vector<int>& vec);
std::string vectorToString(const std::vector<double>& vec);
std::string setToString(const std::unordered_set<int>& set);

// Initialize debug system - call this early in main()
void initializeDebug(int argc, char* argv[]);

#endif // DEBUG_H
