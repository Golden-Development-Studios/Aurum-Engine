#include <Framework/Logger.hpp>
#include <Framework/Config.hpp>
#include <Framework/Timer.hpp>
#include <thread> // for std::this_thread::sleep_for

int main()
{
    using namespace Aurum;

    Logger::Get().SetLogFile("AurumLog.txt");
    Logger::Get().Log("=== Aurum Sandbox with Timer Test ===", LogLevel::Info);

    // --- HighResolutionTimer example ---
    HighResolutionTimer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    Logger::Get().Log("Manual Timer: " + std::to_string(timer.ElapsedMilliseconds()) + " ms");

    // --- ScopedTimer example ---
    {
        ScopedTimer scoped("Scoped Workload");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } // automatically logs upon scope exit

    // --- FrameTimer example ---
    FrameTimer frameTimer;
    for (int i = 0; i < 3; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // simulate 60 FPS
        double delta = frameTimer.Tick();
        Logger::Get().Log("Frame " + std::to_string(i) + " delta: " + std::to_string(delta * 1000.0) + " ms");
    }

    Logger::Get().Log("=== Timer Test Complete ===", LogLevel::Info);
    return 0;
}
