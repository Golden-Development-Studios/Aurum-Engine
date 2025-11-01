# --- Aurum Sandbox Executable ---

add_executable(AurumSandbox
    src/main.cpp
)

# --- Ensure we use C++20 ---
target_compile_features(AurumSandbox PUBLIC cxx_std_20)

# --- Link against the Engine (which links Framework internally) ---
target_link_libraries(AurumSandbox
    PRIVATE
        AurumEngine
)

# --- Include paths ---
target_include_directories(AurumSandbox
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Engine/include
        ${CMAKE_SOURCE_DIR}/src/Framework/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Mark this target as a Windows GUI executable ---
# This tells MSVC that we'll provide WinMain() instead of main()
set_target_properties(AurumSandbox PROPERTIES WIN32_EXECUTABLE TRUE)
