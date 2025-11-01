#pragma once
#include <cmath>
#include <string>
#include <sstream>

namespace Aurum
{
    struct Vector4
    {
        float x, y, z, w;

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Vector4 operator+(const Vector4& v) const { return {x+v.x, y+v.y, z+v.z, w+v.w}; }
        Vector4 operator-(const Vector4& v) const { return {x-v.x, y-v.y, z-v.z, w-v.w}; }
        Vector4 operator*(float s) const { return {x*s, y*s, z*s, w*s}; }

        static float Dot(const Vector4& a, const Vector4& b)
        {
            return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
        }

        std::string ToString() const
        {
            std::ostringstream ss;
            ss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
            return ss.str();
        }
    };
}
