#pragma once
#include <Framework/Logger.hpp>
#include <Engine/TimeSystem.hpp>
#include <string>

namespace Aurum
{
    class DebugOverlay
    {
    public:
        DebugOverlay() = default;

        void Initialize()
        {
            Logger::Get().Log("DebugOverlay initialized.", LogLevel::Info);
        }

        void Update(const TimeSystem& timeSystem, bool logFrameStats = false)
        {
            double delta = timeSystem.GetDeltaTime();
            double fps   = timeSystem.GetFPS();

            if (logFrameStats)
            {
                Logger::Get().Log(
                    "Δt: " + std::to_string(delta) + "s | FPS: " + std::to_string(fps),
                    LogLevel::Debug
                );
            }
        }

        void Shutdown()
        {
            Logger::Get().Log("DebugOverlay shut down.", LogLevel::Info);
        }
    };
}
