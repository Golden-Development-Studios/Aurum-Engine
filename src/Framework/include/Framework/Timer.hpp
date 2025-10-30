#pragma once
#include <chrono>
#include <string>
#include <Framework/Logger.hpp>

namespace Aurum
{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    // ---------------------------------------
    // Basic Timer: manual start/stop control
    // ---------------------------------------
    class HighResolutionTimer
    {
    public:
        HighResolutionTimer() { Reset(); }

        void Reset()
        {
            start_ = Clock::now();
        }

        double ElapsedSeconds() const
        {
            return std::chrono::duration<double>(Clock::now() - start_).count();
        }

        double ElapsedMilliseconds() const
        {
            return std::chrono::duration<double, std::milli>(Clock::now() - start_).count();
        }

        double ElapsedMicroseconds() const
        {
            return std::chrono::duration<double, std::micro>(Clock::now() - start_).count();
        }

    private:
        TimePoint start_;
    };

    // ------------------------------------------------
    // Scoped Timer: logs duration automatically (RAII)
    // ------------------------------------------------
    class ScopedTimer
    {
    public:
        explicit ScopedTimer(const std::string& label)
            : label_(label), start_(Clock::now()) {}

        ~ScopedTimer()
        {
            auto end = Clock::now();
            double ms = std::chrono::duration<double, std::milli>(end - start_).count();
            Logger::Get().Log(label_ + " took " + std::to_string(ms) + " ms", LogLevel::Info);
        }

    private:
        std::string label_;
        TimePoint start_;
    };

    // ---------------------------------------
    // Frame Timer: tracks delta per frame
    // ---------------------------------------
    class FrameTimer
    {
    public:
        FrameTimer() { last_ = Clock::now(); }

        double Tick()
        {
            auto now = Clock::now();
            double delta = std::chrono::duration<double>(now - last_).count();
            last_ = now;
            return delta;
        }

    private:
        TimePoint last_;
    };
}
