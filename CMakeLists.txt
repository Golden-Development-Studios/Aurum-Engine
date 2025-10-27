# --- 1. Require a modern version of CMake ---
cmake_minimum_required(VERSION 3.28)

# --- 2. Define the project ---
project(AurumProject
    VERSION 0.1.0
    DESCRIPTION "Aurum Game Framework and Engine Project"
    LANGUAGES CXX
)

# --- 3. Global C++ standard settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- 4. Default output directories (for all targets) ---
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- 5. Add source directories ---
add_subdirectory(src/Framework)
add_subdirectory(src/Engine)
add_subdirectory(src/Sandbox)
