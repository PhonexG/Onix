cmake_minimum_required(VERSION 3.25)
project(Onix VERSION 0.1.0 LANGUAGES CXX)

# Required C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options for sanitizers
option(ENABLE_SANITIZERS "Enable address and UB sanitizers" ON)

# Basic compiler flags
add_library(onix_compile_options INTERFACE)
target_compile_options(onix_compile_options INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Wnon-virtual-dtor
)

if(ENABLE_SANITIZERS AND (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message(STATUS "AddressSanitizer and UndefinedBehaviorSanitizer: ENABLED")
    target_compile_options(onix_compile_options INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(onix_compile_options INTERFACE -fsanitize=address,undefined)
endif()

# Public project title
include_directories(include)

# Editor core
# Editor core
add_library(onix_core STATIC
    src/core/PieceTable.cpp
    src/core/LineIndex.cpp
    src/core/TextBuffer.cpp
)
target_include_directories(onix_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(onix_core PUBLIC onix_compile_options)

# Enabling testing
enable_testing()
add_subdirectory(tests)

# Main binary
add_executable(onix src/main.cpp)
target_link_libraries(onix PRIVATE onix_core onix_compile_options)
