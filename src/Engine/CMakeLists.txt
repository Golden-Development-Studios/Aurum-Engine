# ============================================================
# Aurum Engine - Core CMake Configuration
# ============================================================

# --- Engine Static Library ---
add_library(AurumEngine STATIC
    # --- Source Files ---
    src/Renderer.cpp
    src/Window.cpp
    src/Application.cpp
    src/Event.cpp
    src/EventDispatcher.cpp
    src/Input.cpp
    src/TimeSystem.cpp
    src/EngineRuntimeConfig.cpp
    src/DebugOverlay.cpp

    # --- DirectX 12 Rendering Backend ---
    render/dx12/D3D12Context.cpp

    # --- Header Files ---
    include/engine/Renderer.hpp
    include/engine/Window.hpp
    include/engine/Application.hpp
    include/engine/Event.hpp
    include/engine/EventDispatcher.hpp
    include/engine/Input.hpp
    include/engine/InputCodes.hpp
    include/engine/TimeSystem.hpp
    include/engine/EngineRuntimeConfig.hpp
    include/engine/DebugOverlay.hpp

    # --- DX12 Context Header ---
    render/dx12/D3D12Context.hpp
)

# ============================================================
# Windows + DirectX 12 Baseline
# ============================================================
if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
    # Require at least Windows 10 (0x0A00)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

# ============================================================
# Include Directories
# ============================================================
target_include_directories(AurumEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/engine
        ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/DirectX-Headers/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/External/AgilitySDK/directx
)

# ============================================================
# Linked Libraries
# ============================================================
target_link_libraries(AurumEngine
    PUBLIC
        AurumFramework
        d3d12
        dxgi
        dxguid
)

# ============================================================
# Compiler Features
# ============================================================
target_compile_features(AurumEngine PUBLIC cxx_std_20)

# ============================================================
# Post-Build: Copy Agility SDK Runtime DLLs
# ============================================================
add_custom_command(TARGET AurumEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:AurumEngine>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/include/External/AgilitySDK/bin/x64/d3d12core.dll"
        "$<TARGET_FILE_DIR:AurumEngine>/d3d12core.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/include/External/AgilitySDK/bin/x64/d3d12SDKLayers.dll"
        "$<TARGET_FILE_DIR:AurumEngine>/d3d12SDKLayers.dll"
    COMMENT "Copying Agility SDK runtime DLLs..."
)

# ============================================================
# Post-Build: Copy Sandbox Runtime Config Files
# ============================================================
add_custom_command(TARGET AurumEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:AurumSandbox>/config"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/Sandbox/config"
        "$<TARGET_FILE_DIR:AurumSandbox>/config"
    COMMENT "Copying Sandbox runtime configuration files..."
)

# ============================================================
# IDE Source Grouping (Optional but Helpful)
# ============================================================
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    src/Renderer.cpp
    src/Window.cpp
    src/Application.cpp
    src/Event.cpp
    src/EventDispatcher.cpp
    src/Input.cpp
    src/TimeSystem.cpp
    src/EngineRuntimeConfig.cpp
    src/DebugOverlay.cpp
    render/dx12/D3D12Context.cpp
    include/engine/Renderer.hpp
    include/engine/Window.hpp
    include/engine/Application.hpp
    include/engine/Event.hpp
    include/engine/EventDispatcher.hpp
    include/engine/Input.hpp
    include/engine/InputCodes.hpp
    include/engine/TimeSystem.hpp
    include/engine/EngineRuntimeConfig.hpp
    include/engine/DebugOverlay.hpp
    render/dx12/D3D12Context.hpp
)
