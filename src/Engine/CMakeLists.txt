# --- Aurum Engine Module ---

add_library(AurumEngine STATIC
    src/Renderer.cpp
    src/Window.cpp
    include/Engine/Renderer.hpp
    include/Engine/Window.hpp
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
