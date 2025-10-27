# --- Framework Module ---

# Create the static library targer
add_library(AurumFramework STATIC
    src/Logger.cpp
    include/Framework/Logger.hpp
)

# Specify include directories for this target
target_include_directories(AurumFramework
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: Set Compiler warning
target_compile_features(AurumFramework PUBLIC cxx_std_20)

# Group soruces (for Visual Studio Organisation)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    src/Logger.cpp
    include/Framework/Logger.hpp
)