# --- Aurum Engine Module ---

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

    # --- Header Files ---
    include/Engine/Renderer.hpp
    include/Engine/Window.hpp
    include/Engine/Application.hpp
    include/Engine/Event.hpp
    include/Engine/EventDispatcher.hpp
    include/Engine/Input.hpp
    include/Engine/InputCodes.hpp
    include/Engine/TimeSystem.hpp
    include/Engine/EngineRuntimeConfig.hpp
)

# --- Include Directories ---
target_include_directories(AurumEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Engine/DirectX-Headers/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/External/AgilitySDK/directx
)

# --- Link Libraries ---
target_link_libraries(AurumEngine
    PUBLIC
        AurumFramework
        d3d12.lib
        dxgi.lib
)

# --- C++ Standard ---
target_compile_features(AurumEngine PUBLIC cxx_std_20)

# --- Post-Build: Copy Agility SDK Runtime DLLs ---
add_custom_command(TARGET AurumEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/include/External/AgilitySDK/bin/x64/d3d12core.dll"
        "$<TARGET_FILE_DIR:AurumEngine>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/include/External/AgilitySDK/bin/x64/d3d12SDKLayers.dll"
        "$<TARGET_FILE_DIR:AurumEngine>"
)

# --- IDE Source Grouping (Optional but Helpful) ---
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    src/Renderer.cpp
    src/Window.cpp
    src/Application.cpp
    src/Event.cpp
    src/EventDispatcher.cpp
    src/Input.cpp
    src/TimeSystem.cpp
    src/EngineRuntimeConfig.cpp
    include/Engine/Renderer.hpp
    include/Engine/Window.hpp
    include/Engine/Application.hpp
    include/Engine/Event.hpp
    include/Engine/EventDispatcher.hpp
    include/Engine/Input.hpp
    include/Engine/InputCodes.hpp
    include/Engine/TimeSystem.hpp
    include/Engine/EngineRuntimeConfig.hpp
)
