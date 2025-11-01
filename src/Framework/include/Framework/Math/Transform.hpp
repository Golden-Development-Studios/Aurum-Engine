#pragma once
#include <Framework/Math/Vector3.hpp>
#include <Framework/Math/Quaternion.hpp>
#include <Framework/Math/Matrix4x4.hpp>

namespace Aurum
{
    struct Transform
    {
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;

        Transform()
            : position(0, 0, 0), rotation(), scale(1, 1, 1) {}

        Matrix4x4 ToMatrix() const
        {
            Matrix4x4 T = Matrix4x4::Translation(position);
            Matrix4x4 S = Matrix4x4::Scale(scale);
            // Rotation matrix from quaternion simplified placeholder (expand later)
            Matrix4x4 R = Matrix4x4::Identity();
            return S * R * T;
        }
    };
}
