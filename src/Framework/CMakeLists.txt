# --- Aurum Framework Module ---

# Create the static library target
add_library(AurumFramework STATIC
    # ---- Source Files ----
    src/Logger.cpp
    src/Config.cpp
    src/Timer.cpp
    src/MemoryTracker.cpp
    src/Math.cpp

    # ---- Header Files ----
    include/Framework/Logger.hpp
    include/Framework/Config.hpp
    include/Framework/Timer.hpp
    include/Framework/MemoryTracker.hpp

    # ---- Math Headers ----
    include/Framework/Math/Math.hpp
    include/Framework/Math/Vector2.hpp
    include/Framework/Math/Vector3.hpp
    include/Framework/Math/Vector4.hpp
    include/Framework/Math/Matrix4x4.hpp
    include/Framework/Math/Quaternion.hpp
    include/Framework/Math/Transform.hpp
)

# --- Include Directories ---
target_include_directories(AurumFramework
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Compiler Features ---
target_compile_features(AurumFramework PUBLIC cxx_std_20)

# --- Source Grouping (IDE Organization) ---
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    src/Logger.cpp
    src/Config.cpp
    src/Timer.cpp
    src/MemoryTracker.cpp
    src/Math.cpp

    include/Framework/Logger.hpp
    include/Framework/Config.hpp
    include/Framework/Timer.hpp
    include/Framework/MemoryTracker.hpp

    include/Framework/Math/Math.hpp
    include/Framework/Math/Vector2.hpp
    include/Framework/Math/Vector3.hpp
    include/Framework/Math/Vector4.hpp
    include/Framework/Math/Matrix4x4.hpp
    include/Framework/Math/Quaternion.hpp
    include/Framework/Math/Transform.hpp
)

# --- Notes ---
# This library now provides:
#   • Core Utilities (Logger, Config, Timer, MemoryTracker)
#   • Math Library (Vectors, Matrix4x4, Quaternion, Transform)
# It serves as the foundational layer for the AurumEngine static library.
