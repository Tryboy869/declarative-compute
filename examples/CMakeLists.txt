cmake_minimum_required(VERSION 3.10)
project(DeclarativeCompute VERSION 1.0.0 LANGUAGES CXX)

# C++17 required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -pthread")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /O2 /EHsc")
endif()

# Include directory
include_directories(${CMAKE_SOURCE_DIR}/../include)

# Examples
add_executable(example_usage example_usage.cpp)

# Optional: Individual examples
add_executable(basic_usage basic_usage.cpp)
add_executable(advanced_usage advanced_usage.cpp)
add_executable(image_processing image_processing.cpp)
add_executable(benchmark benchmark.cpp)

# Threading support
find_package(Threads REQUIRED)
target_link_libraries(example_usage Threads::Threads)
target_link_libraries(basic_usage Threads::Threads)
target_link_libraries(advanced_usage Threads::Threads)
target_link_libraries(image_processing Threads::Threads)
target_link_libraries(benchmark Threads::Threads)

# Install targets (optional)
install(TARGETS example_usage DESTINATION bin)

# Custom target to run examples
add_custom_target(run_examples
    COMMAND example_usage
    DEPENDS example_usage
    COMMENT "Running usage examples..."
)

# Print build info
message(STATUS "Declarative Compute v${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")