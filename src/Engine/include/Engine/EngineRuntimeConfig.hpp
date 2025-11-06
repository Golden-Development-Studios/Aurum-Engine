#pragma once
#include <Framework/Config.hpp>
#include <Framework/Logger.hpp>

namespace Aurum
{
    class EngineRuntimeConfig
    {
    public:
        EngineRuntimeConfig() = default;

        bool Load(const std::string& path)
        {
            // Access the shared ConfigManager instance
            auto& cfg = ConfigManager::Get();

            if (!cfg.Load(path))
            {
                Logger::Get().Log("Runtime config missing: " + path, LogLevel::Warning);
                return false;
            }

            width_       = cfg.GetInt("window.width", 1280);
            height_      = cfg.GetInt("window.height", 720);
            fullscreen_  = cfg.GetBool("window.fullscreen", false);
            targetFPS_   = cfg.GetFloat("render.target_fps", 60.0f);
            vsync_       = cfg.GetBool("render.vsync", true);
            debugLayer_  = cfg.GetBool("render.debug_layer", false);
            showFPS_     = cfg.GetBool("debug.show_fps_overlay", false);

            Logger::Get().Log(
                "Runtime Config Loaded: " + std::to_string(width_) + "x" +
                std::to_string(height_) + " | FPS=" + std::to_string(targetFPS_) +
                " | Fullscreen=" + std::to_string(fullscreen_) +
                " | VSync=" + std::to_string(vsync_),
                LogLevel::Info
            );
            return true;
        }

        // --- Accessors ---
        int  GetWidth()        const { return width_; }
        int  GetHeight()       const { return height_; }
        bool IsFullscreen()    const { return fullscreen_; }
        float GetTargetFPS()   const { return targetFPS_; }
        bool IsVSyncEnabled()  const { return vsync_; }
        bool IsDebugLayer()    const { return debugLayer_; }
        bool ShouldShowFPS()   const { return showFPS_; }

    private:
        int   width_       = 1280;
        int   height_      = 720;
        bool  fullscreen_  = false;
        float targetFPS_   = 60.0f;
        bool  vsync_       = true;
        bool  debugLayer_  = false;
        bool  showFPS_     = false;
    };
}
