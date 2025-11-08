#pragma once
#include <Framework/Timer.hpp>
#include <Framework/Logger.hpp>
#include <thread>
#include <chrono>

namespace Aurum
{
    class TimeSystem
    {
    public:
        TimeSystem()
            : frameCount_(0),
              deltaTime_(0.0),
              fps_(0.0),
              targetFrameTime_(0.0),
              accumulator_(0.0),
              totalTime_(0.0) // ✅ initialize
        {}

        // Initialize with optional target FPS (0 disables frame limiting)
        void Initialize(double targetFPS = 0.0)
        {
            if (targetFPS > 0.0)
                targetFrameTime_ = 1.0 / targetFPS;
        }

        void Tick()
        {
            double dt = frameTimer_.Tick();
            deltaTime_ = dt;
            totalTime_ += dt; // ✅ accumulate total elapsed time
            accumulator_ += dt;
            frameCount_++;

            // --- FPS Calculation (once per second) ---
            if (accumulator_ >= 1.0)
            {
                fps_ = frameCount_ / accumulator_;
                frameCount_ = 0;
                accumulator_ = 0.0;
            }

            // --- Frame Limiter (Hybrid Sleep + Spin) ---
            if (targetFrameTime_ > 0.0)
            {
                using namespace std::chrono;

                double elapsed = frameTimer_.ElapsedSeconds();

                // Coarse sleep until near target
                while (elapsed < targetFrameTime_ - 0.002)
                {
                    std::this_thread::sleep_for(milliseconds(1));
                    elapsed = frameTimer_.ElapsedSeconds();
                }

                // Fine busy-wait for last few microseconds
                while (frameTimer_.ElapsedSeconds() < targetFrameTime_) {}
            }
        }

        // --- Accessors ---
        double GetDeltaTime() const { return deltaTime_; }
        double GetFPS() const { return fps_; }
        double GetTargetFrameTime() const { return targetFrameTime_; }

        // ✅ Added for animated color / global timing
        double GetTotalTime() const { return totalTime_; }

    private:
        FrameTimer frameTimer_;
        double deltaTime_;
        double fps_;
        double targetFrameTime_;
        double accumulator_;
        unsigned int frameCount_;
        double totalTime_; // ✅ new field
    };
}
