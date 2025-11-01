#pragma once
#include <cmath>
#include <string>
#include <sstream>

namespace Aurum
{
    struct Vector3
    {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        // Basic arithmetic
        Vector3 operator+(const Vector3& other) const { return {x + other.x, y + other.y, z + other.z}; }
        Vector3 operator-(const Vector3& other) const { return {x - other.x, y - other.y, z - other.z}; }
        Vector3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
        Vector3 operator/(float scalar) const { return {x / scalar, y / scalar, z / scalar}; }

        Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
        Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

        // Vector operations
        float Length() const { return std::sqrt(x*x + y*y + z*z); }
        float LengthSq() const { return x*x + y*y + z*z; }

        void Normalize()
        {
            float len = Length();
            if (len > 1e-6f)
            {
                x /= len; y /= len; z /= len;
            }
        }

        Vector3 Normalized() const
        {
            float len = Length();
            return (len > 1e-6f) ? Vector3(x/len, y/len, z/len) : Vector3();
        }

        static float Dot(const Vector3& a, const Vector3& b)
        {
            return a.x*b.x + a.y*b.y + a.z*b.z;
        }

        static Vector3 Cross(const Vector3& a, const Vector3& b)
        {
            return {
                a.y*b.z - a.z*b.y,
                a.z*b.x - a.x*b.z,
                a.x*b.y - a.y*b.x
            };
        }

        std::string ToString() const
        {
            std::ostringstream ss;
            ss << "(" << x << ", " << y << ", " << z << ")";
            return ss.str();
        }
    };
}
