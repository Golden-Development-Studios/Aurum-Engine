#include <Framework/Logger.hpp>
#include <Framework/MemoryTracker.hpp>
#include <Framework/Timer.hpp>
#include <thread>

int main()
{
    using namespace Aurum;

    Logger::Get().SetLogFile("AurumLog.txt");
    Logger::Get().Log("=== Aurum Memory Tracker Test ===", LogLevel::Info);

    {
        ScopedTimer scoped("Allocation Block");
        int* a = AURUM_NEW(int, 42);
        double* b = AURUM_NEW(double, 3.14159);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        AURUM_DELETE(a);
        AURUM_DELETE(b);
    }

    MemoryTracker::Get().Report();

    Logger::Get().Log("=== Memory Tracker Test Complete ===", LogLevel::Info);
    return 0;
}
