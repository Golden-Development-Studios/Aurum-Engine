# --- Aurum Engine Module ---

add_library(AurumEngine STATIC
    src/Renderer.cpp
    src/Window.cpp
    include/Engine/Renderer.hpp
    include/Engine/Window.hpp
)

target_include_directories(AurumEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link to the Framework
target_link_libraries(AurumEngine
    PUBLIC
        AurumFramework
)

target_compile_features(AurumEngine PUBLIC cxx_std_20)
