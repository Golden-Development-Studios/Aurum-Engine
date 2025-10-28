// main.cpp

#include <Framework/Logger.hpp>

int main()
{
    using namespace Aurum;

    Logger::Get().SetLogFile("AurumLog.txt");
    Logger::Get().Log("Aurum Framework initialized.", LogLevel::Info);
    Logger::Get().Log("This is a warning test.", LogLevel::Warning);
    Logger::Get().Log("This is an error message.", LogLevel::Error);
    Logger::Get().Log("Debug message here.", LogLevel::Debug);

    return 0;
}
