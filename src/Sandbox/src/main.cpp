#include <Framework/Logger.hpp>
#include <Framework/Math/Vector3.hpp>

using namespace Aurum;

int main()
{
    Logger::Get().SetLogFile("AurumLog.txt");
    Logger::Get().Log("=== Aurum Math Library Test ===", LogLevel::Info);

    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);

    Vector3 sum = a + b;
    Vector3 cross = Vector3::Cross(a, b);
    float dot = Vector3::Dot(a, b);

    Logger::Get().Log("a + b = " + sum.ToString());
    Logger::Get().Log("Dot(a, b) = " + std::to_string(dot));
    Logger::Get().Log("Cross(a, b) = " + cross.ToString());

    Logger::Get().Log("=== Math Library Test Complete ===", LogLevel::Info);
    return 0;
}
