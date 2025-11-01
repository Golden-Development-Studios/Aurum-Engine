#include <Framework/Logger.hpp>
#include <Framework/Math/Vector2.hpp>
#include <Framework/Math/Vector3.hpp>
#include <Framework/Math/Vector4.hpp>
#include <Framework/Math/Matrix4x4.hpp>
#include <Framework/Math/Quaternion.hpp>
#include <Framework/Math/Transform.hpp>
#include <cmath>

using namespace Aurum;

int main()
{
    Logger::Get().SetLogFile("AurumLog.txt");
    Logger::Get().Log("=== Aurum Math Library Integration Test ===", LogLevel::Info);

    // -------------------------------------------------------
    // Vector2 Tests
    // -------------------------------------------------------
    Logger::Get().Log("--- Vector2 Tests ---");
    Vector2 v2a(3, 4);
    Vector2 v2b(1, 2);
    Logger::Get().Log("v2a = " + v2a.ToString());
    Logger::Get().Log("v2b = " + v2b.ToString());
    Logger::Get().Log("v2a + v2b = " + (v2a + v2b).ToString());
    Logger::Get().Log("Dot(v2a, v2b) = " + std::to_string(Vector2::Dot(v2a, v2b)));
    Logger::Get().Log("Length(v2a) = " + std::to_string(v2a.Length()));

    // -------------------------------------------------------
    // Vector3 Tests
    // -------------------------------------------------------
    Logger::Get().Log("--- Vector3 Tests ---");
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 sum = a + b;
    Vector3 cross = Vector3::Cross(a, b);
    float dot = Vector3::Dot(a, b);

    Logger::Get().Log("a = " + a.ToString());
    Logger::Get().Log("b = " + b.ToString());
    Logger::Get().Log("a + b = " + sum.ToString());
    Logger::Get().Log("Dot(a, b) = " + std::to_string(dot));
    Logger::Get().Log("Cross(a, b) = " + cross.ToString());
    Logger::Get().Log("Normalized(a) = " + a.Normalized().ToString());

    // -------------------------------------------------------
    // Vector4 Tests
    // -------------------------------------------------------
    Logger::Get().Log("--- Vector4 Tests ---");
    Vector4 v4a(1, 2, 3, 1);
    Vector4 v4b(2, 1, 0, 1);
    Logger::Get().Log("v4a = " + v4a.ToString());
    Logger::Get().Log("v4b = " + v4b.ToString());
    Logger::Get().Log("v4a + v4b = " + (v4a + v4b).ToString());
    Logger::Get().Log("Dot(v4a, v4b) = " + std::to_string(Vector4::Dot(v4a, v4b)));

    // -------------------------------------------------------
    // Matrix4x4 Tests
    // -------------------------------------------------------
    Logger::Get().Log("--- Matrix4x4 Tests ---");
    Matrix4x4 identity = Matrix4x4::Identity();
    Logger::Get().Log("Identity Matrix:\n" + identity.ToString());

    Matrix4x4 translate = Matrix4x4::Translation(Vector3(5, 0, 0));
    Matrix4x4 scale = Matrix4x4::Scale(Vector3(2, 2, 2));
    Matrix4x4 combined = scale * translate;
    Logger::Get().Log("Scale * Translation:\n" + combined.ToString());

    Vector3 point(1, 2, 3);
    Vector3 transformed = combined.TransformPoint(point);
    Logger::Get().Log("TransformPoint(1,2,3) -> " + transformed.ToString());

    // -------------------------------------------------------
    // Quaternion Tests
    // -------------------------------------------------------
    Logger::Get().Log("--- Quaternion Tests ---");
    Vector3 axis(0, 1, 0);
    float angle = 3.14159265f / 2.0f; // 90 degrees
    Quaternion q = Quaternion::FromAxisAngle(axis, angle);
    Logger::Get().Log("Quaternion from Y-axis 90°: (w,x,y,z) = (" +
        std::to_string(q.w) + ", " + std::to_string(q.x) + ", " +
        std::to_string(q.y) + ", " + std::to_string(q.z) + ")");

    Quaternion qNorm = q.Normalized();
    Logger::Get().Log("Normalized Quaternion length check: " +
        std::to_string(std::sqrt(qNorm.w*qNorm.w + qNorm.x*qNorm.x + qNorm.y*qNorm.y + qNorm.z*qNorm.z)));

    // -------------------------------------------------------
    // Transform Tests
    // -------------------------------------------------------
    Logger::Get().Log("--- Transform Tests ---");
    Transform t;
    t.position = Vector3(2, 3, 4);
    t.scale = Vector3(2, 2, 2);
    Matrix4x4 mat = t.ToMatrix();
    Logger::Get().Log("Transform matrix:\n" + mat.ToString());

    Logger::Get().Log("=== Math Library Integration Test Complete ===", LogLevel::Info);
    return 0;
}
