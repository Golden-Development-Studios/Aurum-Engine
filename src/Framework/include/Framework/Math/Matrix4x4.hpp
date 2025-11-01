#pragma once
#include <cstring>
#include <cmath>
#include <sstream>
#include <Framework/Math/Vector3.hpp>

namespace Aurum
{
    struct Matrix4x4
    {
        float m[4][4]; // Row-major

        Matrix4x4() { SetIdentity(); }

        void SetIdentity()
        {
            std::memset(m, 0, sizeof(m));
            m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
        }

        static Matrix4x4 Identity()
        {
            Matrix4x4 mat;
            mat.SetIdentity();
            return mat;
        }

        static Matrix4x4 Translation(const Vector3& v)
        {
            Matrix4x4 mat = Identity();
            mat.m[3][0] = v.x;
            mat.m[3][1] = v.y;
            mat.m[3][2] = v.z;
            return mat;
        }

        static Matrix4x4 Scale(const Vector3& v)
        {
            Matrix4x4 mat = Identity();
            mat.m[0][0] = v.x;
            mat.m[1][1] = v.y;
            mat.m[2][2] = v.z;
            return mat;
        }

        Matrix4x4 operator*(const Matrix4x4& other) const
        {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    result.m[i][j] = 0.0f;
                    for (int k = 0; k < 4; ++k)
                        result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
            return result;
        }

        Vector3 TransformPoint(const Vector3& v) const
        {
            Vector3 r;
            r.x = v.x*m[0][0] + v.y*m[1][0] + v.z*m[2][0] + m[3][0];
            r.y = v.x*m[0][1] + v.y*m[1][1] + v.z*m[2][1] + m[3][1];
            r.z = v.x*m[0][2] + v.y*m[1][2] + v.z*m[2][2] + m[3][2];
            return r;
        }

        std::string ToString() const
        {
            std::ostringstream ss;
            for (int i = 0; i < 4; ++i)
            {
                ss << "[ ";
                for (int j = 0; j < 4; ++j)
                    ss << m[i][j] << " ";
                ss << "]\n";
            }
            return ss.str();
        }
    };
}
