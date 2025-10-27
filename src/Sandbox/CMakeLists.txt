# --- Sandbox Executable ---

add_executable(AurumSandbox
    src/main.cpp
)

target_link_libraries(AurumSandbox
    PRIVATE
        AurumEngine
)

target_include_directories(AurumSandbox
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
