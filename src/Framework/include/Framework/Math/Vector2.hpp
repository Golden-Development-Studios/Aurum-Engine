#pragma once
#include <cmath>
#include <string>
#include <sstream>

namespace Aurum
{
    struct Vector2
    {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}

        Vector2 operator+(const Vector2& v) const { return {x + v.x, y + v.y}; }
        Vector2 operator-(const Vector2& v) const { return {x - v.x, y - v.y}; }
        Vector2 operator*(float s) const { return {x * s, y * s}; }
        Vector2 operator/(float s) const { return {x / s, y / s}; }

        float Length() const { return std::sqrt(x*x + y*y); }
        Vector2 Normalized() const
        {
            float len = Length();
            return len > 1e-6f ? Vector2(x/len, y/len) : Vector2();
        }

        static float Dot(const Vector2& a, const Vector2& b) { return a.x*b.x + a.y*b.y; }

        std::string ToString() const
        {
            std::ostringstream ss;
            ss << "(" << x << ", " << y << ")";
            return ss.str();
        }
    };
}
