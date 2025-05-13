#include "Light.h"

Light::Light(const std::string& filePath)
    :
    mLookAt(0.0f, 0.0f, 0.0f),
    mViewMatrix(XMMatrixIdentity()),
    mPerspectiveProjectionMatrix(XMMatrixIdentity()),
    mOrthographicProjectionMatrix(XMMatrixIdentity())
{
	modelPath = filePath;
}

Light::~Light() noexcept
{
}

void Light::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	SceneGeometry::Init(pDevice, pDeviceContext, modelPath, texturePath);

    GenerateViewMatrix();
    GenerateOrthographicProjectionMatrix(100.0f, 100.0f, 0.1f, 500.0f);
}

void Light::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
    // if directional light is moving
    //GenerateViewMatrix();
    //GenerateOrthographicProjectionMatrix(100.0f, 100.0f, 0.1f, 100.0f);
}

void Light::Draw(const XMMATRIX& viewProjectionMatrix)
{
	SceneGeometry::Draw(viewProjectionMatrix);
}

void Light::SetLookAt(float x, float y, float z)
{
    mLookAt = XMFLOAT3(x, y, z);
}

void Light::GenerateViewMatrix()
{
    XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
    XMVECTOR lookAtVector, upVector;
    XMVECTOR pos = GetPosition();;
    
    // Load the XMFLOAT3 into XMVECTOR.
    lookAtVector = XMLoadFloat3(&mLookAt);
    upVector = XMLoadFloat3(&up);

    // Create the view matrix from the three vectors.
    mViewMatrix = XMMatrixLookAtLH(pos, lookAtVector, upVector);
}

void Light::GeneratePerspectiveProjectionMatrix(float, float)
{
}

void Light::GenerateOrthographicProjectionMatrix(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
    mOrthographicProjectionMatrix = XMMatrixOrthographicLH(ViewWidth, ViewHeight, NearZ, FarZ);
}

const XMMATRIX& Light::GetViewMatrix() const
{
	return mViewMatrix;
}

const XMMATRIX& Light::GetPerspectiveProjectionMatrix() const
{
	return mPerspectiveProjectionMatrix;
}

const XMMATRIX& Light::GetOrthographicProjectionMatrix() const
{
    return mOrthographicProjectionMatrix;
}