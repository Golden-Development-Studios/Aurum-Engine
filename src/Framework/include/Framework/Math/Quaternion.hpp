#pragma once
#include <cmath>
#include <Framework/Math/Vector3.hpp>

namespace Aurum
{
    struct Quaternion
    {
        float w, x, y, z;

        Quaternion() : w(1), x(0), y(0), z(0) {}
        Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

        static Quaternion FromAxisAngle(const Vector3& axis, float angleRad)
        {
            float half = angleRad * 0.5f;
            float s = std::sin(half);
            return {std::cos(half), axis.x * s, axis.y * s, axis.z * s};
        }

        Quaternion Normalized() const
        {
            float len = std::sqrt(w*w + x*x + y*y + z*z);
            return {w/len, x/len, y/len, z/len};
        }

        static Quaternion Multiply(const Quaternion& a, const Quaternion& b)
        {
            return {
                a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
                a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
                a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
                a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w
            };
        }
    };
}
