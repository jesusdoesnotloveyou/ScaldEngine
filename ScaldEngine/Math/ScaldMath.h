#pragma once

namespace Scald
{
    struct Vector3;
    struct Quaternion;
    struct Matrix4;
    
    // FLOAT specific
    struct Vector3
    {
        float x, y, z;

        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3(float s) : x(s), y(s), z(s) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        // Inline cause no need for specific math library
        Vector3 operator+(const Vector3& lhs) const 
        { 
            return Vector3(x+lhs.x, y+lhs.y, z+lhs.z);
        }

        Vector3 operator-(const Vector3& lhs) const
        {
            return Vector3(x-lhs.x, y-lhs.y, z-lhs.z);
        }

        Vector3 operator*(float s) const
        {
            return Vector3(x*s, y*s, z*s);
        }

        float Dot(const Vector3& rhs) const
        {
            return x*rhs.x + y*rhs.y + z*rhs.z;
        }

        float LengthSq() const { return Dot(*this); }

        float Length() const;
        // float Length2D() const; // XOY or XOZ
        Vector3 Normalized() const;
        Vector3 Cross(const Vector3& rhs) const;

        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Forward;
        static const Vector3 Up;
        static const Vector3 Right;
    };

    struct Quaternion
    {
        float x, y, z, w;

        Quaternion()
            : x(0.0f)
            , y(0.0f)
            , z(0.0f)
            , w(1.0f)
        {}
        Quaternion(const Quaternion& q) = default;
        Quaternion(Quaternion&& q) noexcept = default;
        Quaternion& operator=(const Quaternion& q) = default;
        Quaternion& operator=(Quaternion&& q) noexcept = default;

        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Quaternion  Normalized()                      const;
        Quaternion  Inverse()                         const;
        Vector3     RotateVector(const Vector3& v)    const;
        Matrix4     ToMatrix()                        const;

        static Quaternion FromEuler(float pitch, float yaw, float roll);
        static Quaternion FromAxisAngle(const Vector3& axis, float angleDeg);

        static const Quaternion Identity;
    };

    struct Matrix4
    {
        union
        {
            struct
            {
                float m00, m01, m02, m03;
                float m10, m11, m12, m13;
                float m20, m21, m22, m23;
                float m30, m31, m32, m33;
            };
            float m[4][4];
        };
        
        constexpr Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33
        );

        Matrix4(const Matrix4& mat) = default;
        Matrix4(Matrix4&& mat) noexcept = default;
        Matrix4& operator=(const Matrix4& mat) = default;
        Matrix4& operator=(Matrix4&& mat) noexcept = default;

        Matrix4 operator*(const Matrix4& rhs)   const;
        Vector3 TransformPoint(const Vector3& p) const;
        Vector3 TransformVector(const Vector3& v) const;
        Matrix4 Transpose()                     const;
        Matrix4 Inverse()                       const;

        static Matrix4 Translation(const Vector3& t);
        static Matrix4 Rotation(const Quaternion& q);
        static Matrix4 Scale(const Vector3& s);
        static Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
        static Matrix4 Perspective(float fovY, float aspect, float nearZ, float farZ);

        static const Matrix4 Identity;
    };
}