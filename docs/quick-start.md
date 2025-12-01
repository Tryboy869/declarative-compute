# Quick Start Guide

Get started with Declarative Compute in 5 minutes!

## Installation

### Step 1: Copy the Header

```bash
# Copy the single header file to your project
cp include/declarative_compute.hpp /your/project/include/
```

That's it! Header-only library, no linking required.

### Step 2: Include in Your Code

```cpp
#include "declarative_compute.hpp"
```

## Your First Program

### 1. Basic Example (Simplest)

```cpp
#include "declarative_compute.hpp"
#include <iostream>
#include <vector>

int main() {
    // Your data
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    // Process with automatic optimization
    auto result = declarative::process(numbers, [](int x) {
        return x * x;  // Square each number
    });
    
    // Check results
    std::cout << "Processed: " << result.items_processed << " items\n";
    std::cout << "Time: " << result.execution_time_ms << " ms\n";
    
    // Print results
    for (const auto& val : result.results) {
        std::cout << val << " ";
    }
    
    return 0;
}
```

### 2. Compile and Run

```bash
# GCC/Clang
g++ -std=c++17 -pthread -O3 your_program.cpp -o program
./program

# Output:
# Processed: 5 items
# Time: 0.123 ms
# 1 4 9 16 25
```

## Configuration Options

### Adaptive Mode (Recommended)

Let the library choose the best strategy automatically:

```cpp
auto result = declarative::process(data, [](int x) {
    return x * 2;
});

// Library automatically:
// - Uses sequential for small datasets
// - Uses parallel for large datasets
// - Optimizes thread count
```

### Manual Configuration

Take control when needed:

```cpp
declarative::ProcessConfig config;
config.memory = declarative::MemoryPolicy::Pooled;
config.concurrency = declarative::ConcurrencyPolicy::Parallel;
config.max_threads = 8;

auto result = declarative::process(data, config, function);
```

## Common Use Cases

### Data Processing

```cpp
std::vector<double> data = load_data();

auto result = declarative::process<double, double>(
    data,
    {.concurrency = ConcurrencyPolicy::Adaptive},
    [](double x) {
        return std::sqrt(x * x + 1.0);
    }
);
```

### Image Processing

```cpp
std::vector<Pixel> image = load_image("photo.jpg");

auto result = declarative::process<Pixel, Pixel>(
    image,
    {.concurrency = ConcurrencyPolicy::Parallel},
    apply_filter
);
```

### Scientific Computing

```cpp
std::vector<double> matrix(1000000);

auto result = declarative::process<double, double>(
    matrix,
    {.max_threads = 16},
    expensive_computation
);
```

## Performance Tips

### 1. Use Adaptive for Unknown Workloads

```cpp
config.concurrency = ConcurrencyPolicy::Adaptive;
// Library chooses optimal strategy
```

### 2. Use Pooled Memory for Many Allocations

```cpp
config.memory = MemoryPolicy::Pooled;
// Reuses memory instead of allocating/deallocating
```

### 3. Tune Thread Count

```cpp
config.max_threads = std::thread::hardware_concurrency();
// Use all available cores
```

### 4. Benchmark Your Workload

```cpp
auto bench = declarative::benchmark(data, function);
std::cout << "Best strategy: ";
if (bench.speedup_parallel > 2.0) {
    std::cout << "Parallel\n";
} else {
    std::cout << "Sequential\n";
}
```

## Error Handling

```cpp
auto result = declarative::process(data, config, function);

if (!result.success) {
    std::cerr << "Error: " << result.error_message << "\n";
    return 1;
}

// Process results
for (const auto& item : result.results) {
    std::cout << item << "\n";
}
```

## Next Steps

- Read [API Reference](api-reference.md) for all options
- Check [examples/](../examples/) for more use cases
- See [README.md](../README.md) for advanced features

## Troubleshooting

### Issue: Compilation Error

```
error: 'thread' is not a member of 'std'
```

**Solution:** Add `-pthread` flag:
```bash
g++ -std=c++17 -pthread your_program.cpp
```

### Issue: Performance Not Improving

**Check:** Dataset size
```cpp
// For small datasets (< 1000 items), sequential might be faster
// Use Adaptive mode to let library decide
```

### Issue: High Memory Usage

**Solution:** Use memory pooling
```cpp
config.memory = MemoryPolicy::Pooled;
```

## Support

- Email: support@declarativecompute.com
- Documentation: See README.md
- Examples: Check examples/ directory

Happy coding! 🚀