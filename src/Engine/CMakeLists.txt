# --- Engine Module ---

add_library(AurumEngine STATIC
    src/EngineCore.cpp
    include/Engine/EngineCore.hpp
)

target_include_directories(AurumEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against the Framework
target_link_libraries(AurumEngine
    PUBLIC
        AurumFramework
)

target_compile_features(AurumEngine PUBLIC cxx_std_20)
