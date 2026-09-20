#include "ScaldMath.h"
#include "DirectXMath.h"

using namespace DirectX;
using namespace Scald;

// DirectX left-handed specific
const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);

const Matrix4 Matrix4::Identity = 
{
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
};

float Vector3::Length() const
{
    return 0.0f;
}

// float Length2D() const; // XOY or XOZ
Vector3 Vector3::Normalized() const
{
    return Vector3();
}

Vector3 Vector3::Cross(const Vector3& rhs) const
{
    return Vector3();
}

constexpr Matrix4::Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33)
            : m00(m00), m01(m01), m02(m02), m03(m03)
            , m10(m10), m11(m11), m12(m12), m13(m13)
            , m20(m20), m21(m21), m22(m22), m23(m23)
            , m30(m30), m31(m31), m32(m32), m33(m33)
{
}