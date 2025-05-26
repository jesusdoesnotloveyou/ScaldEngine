#include "Light.h"

Light::Light(const std::string& filePath)
    :
    mLookAt(0.0f, 0.0f, 0.0f),
    mViewMatrix(XMMatrixIdentity()),
    mPerspectiveProjectionMatrix(XMMatrixIdentity()),
    mOrthographicProjectionMatrix(XMMatrixIdentity())
{
    LightParams = new LIGHT_DESC{};
	modelPath = filePath;
}

Light::~Light() noexcept
{
    if (LightParams) delete LightParams;
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

void Light::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	SceneGeometry::Draw(viewMatrix, projectionMatrix);
}

void Light::SetAmbientColor(float x, float y, float z, float w)
{
    if (!LightParams) return;
    LightParams->ambient = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 Light::GetAmbientColor()
{
    return LightParams ? LightParams->ambient : XMFLOAT4{};
}

void Light::SetDiffuseColor(float x, float y, float z, float w)
{
    if (!LightParams) return;
    LightParams->diffuse = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 Light::GetDiffuseColor()
{
    return LightParams ? LightParams->diffuse : XMFLOAT4{};
}

void Light::SetSpecularColor(float x, float y, float z, float w)
{
    if (!LightParams) return;
    LightParams->specular = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 Light::GetSpecularColor()
{
    return LightParams ? LightParams->specular : XMFLOAT4{};
}

void Light::SetLookAt(float x, float y, float z)
{
    mLookAt = XMFLOAT3(x, y, z);
}

void Light::SetDirection(float x, float y, float z)
{
    if (!LightParams) return;
    XMStoreFloat3(&LightParams->direction, XMVector3Normalize(XMVectorSet(x, y, z, 0.0f)));
}

XMFLOAT3 Light::GetDirection()
{
    return LightParams ? LightParams->direction : XMFLOAT3{};
}

void Light::SetAttenuation(float x, float y, float z)
{
    if (!LightParams) return;
    LightParams->attenuation = XMFLOAT3(x, y, z);
}

XMFLOAT3 Light::GetAttenuation()
{
    return LightParams ? LightParams->attenuation : XMFLOAT3{};
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

void Light::UpdateLightParams()
{

}