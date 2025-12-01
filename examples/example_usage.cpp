/**
 * ============================================================================
 * DECLARATIVE COMPUTE - Usage Examples
 * ============================================================================
 * 
 * This file demonstrates all features of the Declarative Compute library
 * Compile with: g++ -std=c++17 -O3 -pthread example_usage.cpp -o example
 * ============================================================================
 */

#include "declarative_compute.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

// ============================================================================
// EXAMPLE 1: Basic Usage (Simplest Case)
// ============================================================================

void example_basic() {
    std::cout << "\n=== EXAMPLE 1: Basic Usage ===\n\n";
    
    // Generate test data
    std::vector<int> numbers(10000);
    std::iota(numbers.begin(), numbers.end(), 1);
    
    // Process with automatic optimization
    auto result = declarative::process(numbers, [](int x) {
        return x * x; // Square each number
    });
    
    std::cout << "Processed " << result.items_processed << " items\n";
    std::cout << "Execution time: " << result.execution_time_ms << " ms\n";
    std::cout << "Threads used: " << result.threads_used << "\n";
    std::cout << "Success: " << (result.success ? "Yes" : "No") << "\n";
    
    // Show first 10 results
    std::cout << "\nFirst 10 results: ";
    for (size_t i = 0; i < 10 && i < result.results.size(); ++i) {
        std::cout << result.results[i] << " ";
    }
    std::cout << "\n";
}

// ============================================================================
// EXAMPLE 2: Declarative Configuration
// ============================================================================

void example_declarative_config() {
    std::cout << "\n=== EXAMPLE 2: Declarative Configuration ===\n\n";
    
    // Generate larger dataset
    std::vector<double> data(100000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 100.0);
    
    for (auto& val : data) {
        val = dis(gen);
    }
    
    // Configure processing declaratively
    declarative::ProcessConfig config;
    config.memory = declarative::MemoryPolicy::Pooled;
    config.concurrency = declarative::ConcurrencyPolicy::Parallel;
    config.safety = declarative::SafetyPolicy::Guaranteed;
    config.max_threads = 8;
    
    // Process with heavy computation
    auto result = declarative::process<double, double>(
        data, 
        config,
        [](double x) {
            // Expensive computation
            double result = x;
            for (int i = 0; i < 100; ++i) {
                result = std::sin(result) * std::cos(result);
            }
            return result;
        }
    );
    
    std::cout << "Items processed: " << result.items_processed << "\n";
    std::cout << "Execution time: " << result.execution_time_ms << " ms\n";
    std::cout << "Threads used: " << result.threads_used << "\n";
    std::cout << "Throughput: " 
              << (result.items_processed / (result.execution_time_ms / 1000.0))
              << " items/sec\n";
}

// ============================================================================
// EXAMPLE 3: Comparison Sequential vs Parallel
// ============================================================================

void example_comparison() {
    std::cout << "\n=== EXAMPLE 3: Sequential vs Parallel Comparison ===\n\n";
    
    std::vector<int> data(50000);
    std::iota(data.begin(), data.end(), 1);
    
    // Heavy computation
    auto heavy_task = [](int x) -> long long {
        long long result = x;
        for (int i = 0; i < 1000; ++i) {
            result = (result * 31 + i) % 1000000007;
        }
        return result;
    };
    
    // Sequential processing
    declarative::ProcessConfig seq_config;
    seq_config.concurrency = declarative::ConcurrencyPolicy::Sequential;
    
    auto seq_result = declarative::process<int, long long>(
        data, seq_config, heavy_task
    );
    
    std::cout << "Sequential:\n";
    std::cout << "  Time: " << seq_result.execution_time_ms << " ms\n";
    std::cout << "  Threads: " << seq_result.threads_used << "\n";
    
    // Parallel processing
    declarative::ProcessConfig par_config;
    par_config.concurrency = declarative::ConcurrencyPolicy::Parallel;
    par_config.max_threads = std::thread::hardware_concurrency();
    
    auto par_result = declarative::process<int, long long>(
        data, par_config, heavy_task
    );
    
    std::cout << "\nParallel:\n";
    std::cout << "  Time: " << par_result.execution_time_ms << " ms\n";
    std::cout << "  Threads: " << par_result.threads_used << "\n";
    
    // Speedup
    double speedup = seq_result.execution_time_ms / par_result.execution_time_ms;
    std::cout << "\nSpeedup: " << std::fixed << std::setprecision(2) 
              << speedup << "x\n";
    std::cout << "Efficiency: " 
              << (speedup / par_result.threads_used * 100.0) << "%\n";
}

// ============================================================================
// EXAMPLE 4: Adaptive Strategy (Automatic Selection)
// ============================================================================

void example_adaptive() {
    std::cout << "\n=== EXAMPLE 4: Adaptive Strategy ===\n\n";
    
    auto test_adaptive = [](size_t size) {
        std::vector<int> data(size);
        std::iota(data.begin(), data.end(), 1);
        
        declarative::ProcessConfig config;
        config.concurrency = declarative::ConcurrencyPolicy::Adaptive;
        
        auto result = declarative::process<int, int>(
            data,
            config,
            [](int x) { return x * 2; }
        );
        
        std::cout << "Size " << std::setw(7) << size 
                  << " -> " << std::setw(2) << result.threads_used << " threads, "
                  << std::setw(8) << std::fixed << std::setprecision(3)
                  << result.execution_time_ms << " ms\n";
    };
    
    std::cout << "Adaptive strategy automatically selects optimal approach:\n\n";
    
    test_adaptive(100);      // Small: sequential
    test_adaptive(500);      // Medium: sequential
    test_adaptive(1000);     // Threshold: may parallelize
    test_adaptive(5000);     // Large: parallel
    test_adaptive(10000);    // Very large: parallel
    test_adaptive(50000);    // Huge: parallel with optimal threads
}

// ============================================================================
// EXAMPLE 5: Real-World Use Case - Image Processing
// ============================================================================

struct Pixel {
    uint8_t r, g, b;
};

void example_image_processing() {
    std::cout << "\n=== EXAMPLE 5: Image Processing Simulation ===\n\n";
    
    // Simulate 4K image (3840x2160 pixels)
    const size_t WIDTH = 3840;
    const size_t HEIGHT = 2160;
    std::vector<Pixel> image(WIDTH * HEIGHT);
    
    // Initialize with random colors
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (auto& pixel : image) {
        pixel.r = dis(gen);
        pixel.g = dis(gen);
        pixel.b = dis(gen);
    }
    
    std::cout << "Processing " << (WIDTH * HEIGHT) << " pixels (4K resolution)\n\n";
    
    // Apply grayscale filter
    declarative::ProcessConfig config;
    config.concurrency = declarative::ConcurrencyPolicy::Parallel;
    config.max_threads = std::thread::hardware_concurrency();
    
    auto result = declarative::process<Pixel, Pixel>(
        image,
        config,
        [](const Pixel& p) -> Pixel {
            // Grayscale conversion
            uint8_t gray = static_cast<uint8_t>(
                0.299 * p.r + 0.587 * p.g + 0.114 * p.b
            );
            return {gray, gray, gray};
        }
    );
    
    std::cout << "Filter applied in " << result.execution_time_ms << " ms\n";
    std::cout << "Processing rate: " 
              << (result.items_processed / (result.execution_time_ms / 1000.0) / 1000000.0)
              << " megapixels/sec\n";
    std::cout << "Threads utilized: " << result.threads_used << "\n";
}

// ============================================================================
// EXAMPLE 6: Benchmark Utility
// ============================================================================

void example_benchmark() {
    std::cout << "\n=== EXAMPLE 6: Automatic Benchmarking ===\n\n";
    
    std::vector<int> data(10000);
    std::iota(data.begin(), data.end(), 1);
    
    auto task = [](int x) -> double {
        double result = x;
        for (int i = 0; i < 100; ++i) {
            result = std::sqrt(result + i);
        }
        return result;
    };
    
    std::cout << "Running benchmark (3 iterations each)...\n\n";
    
    auto bench = declarative::benchmark(data, task, 3);
    
    std::cout << "Results:\n";
    std::cout << "  Sequential: " << std::fixed << std::setprecision(2)
              << bench.sequential_ms << " ms\n";
    std::cout << "  Parallel:   " << bench.parallel_ms << " ms "
              << "(" << bench.speedup_parallel << "x speedup)\n";
    std::cout << "  Adaptive:   " << bench.adaptive_ms << " ms "
              << "(" << bench.speedup_adaptive << "x speedup)\n";
    std::cout << "\nOptimal threads: " << bench.optimal_threads << "\n";
    
    // Recommendation
    if (bench.speedup_parallel > 1.5) {
        std::cout << "\n✅ Recommendation: Use Parallel mode for this workload\n";
    } else if (bench.speedup_parallel > 1.1) {
        std::cout << "\n⚠️  Recommendation: Parallel provides modest gains\n";
    } else {
        std::cout << "\n❌ Recommendation: Sequential mode is optimal\n";
    }
}

// ============================================================================
// EXAMPLE 7: Error Handling
// ============================================================================

void example_error_handling() {
    std::cout << "\n=== EXAMPLE 7: Error Handling ===\n\n";
    
    std::vector<int> data = {1, 2, 3, 4, 5, 0, 7, 8};
    
    auto result = declarative::process<int, double>(
        data,
        declarative::ProcessConfig{},
        [](int x) -> double {
            if (x == 0) {
                throw std::runtime_error("Division by zero");
            }
            return 100.0 / x;
        }
    );
    
    if (!result.success) {
        std::cout << "❌ Processing failed!\n";
        std::cout << "Error: " << result.error_message << "\n";
        std::cout << "Items processed before error: " << result.items_processed << "\n";
    } else {
        std::cout << "✅ Processing successful\n";
        std::cout << "Results: ";
        for (const auto& val : result.results) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     DECLARATIVE COMPUTE - C++ Library Examples            ║\n";
    std::cout << "║     Modern Resource Management with Zero Overhead         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    std::cout << "\nSystem Info:\n";
    std::cout << "  Hardware threads: " << std::thread::hardware_concurrency() << "\n";
    
    try {
        example_basic();
        example_declarative_config();
        example_comparison();
        example_adaptive();
        example_image_processing();
        example_benchmark();
        example_error_handling();
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Fatal error: " << e.what() << "\n";
        return 1;
    }
    
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    All Examples Complete                   ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    
    return 0;
}