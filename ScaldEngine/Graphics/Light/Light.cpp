#include "stdafx.h"
#include "Light.h"
#include "Objects/Geometry/3D/Shapes.h"

Light::Light(const std::string& filePath)
    :
    mLookAt(0.0f, 0.0f, 0.0f),
    mViewMatrix(XMMatrixIdentity()),
    mPerspectiveProjectionMatrix(XMMatrixIdentity()),
    mOrthographicProjectionMatrix(XMMatrixIdentity())
{
    LightParams = std::make_unique<LIGHT_DESC>();
	modelPath = filePath;
    mCollisionComponent->DisableCollision();
}

Light::~Light() noexcept
{
}

void Light::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
    UpdateLightParams();
    //if (LightType == ELightType::Directional)
    //{
    //    // CreateScreenQuad
    //}
    
    if (LightType == ELightType::Point || LightType == ELightType::Spot)
    {
        std::vector<VertexTex> volumeVertices;
        std::vector<DWORD> volumeIndices;
        Shapes::GetSphereShape(volumeVertices, volumeIndices, 1.0f /*hard - coded value just for now*/, 8, 16);

        LightVolume = std::make_unique<Mesh>(pDevice, pDeviceContext, volumeVertices, volumeIndices);
    }

	SceneGeometry::Init(pDevice, pDeviceContext, modelPath, texturePath);

    GenerateViewMatrix();
    GenerateOrthographicProjectionMatrix(100.0f, 100.0f, 0.1f, 500.0f);
}

void Light::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
    UpdateLightParams();
    // if directional light is moving
    //GenerateViewMatrix();
    //GenerateOrthographicProjectionMatrix(100.0f, 100.0f, 0.1f, 100.0f);
}

void Light::Draw()
{
	SceneGeometry::Draw();
}

void Light::DrawLightVolume(ID3D11DeviceContext* pDeviceContext)
{
    // might be good idea to encapsulate light mesh draw here whatever kind of light it is
    // light volume could be sphere, cone or quad
    auto& lightVB = LightVolume->GetVertexBuffer();
    auto& lightIB = LightVolume->GetIndexBuffer();

    pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pDeviceContext->IASetVertexBuffers(0u, 1u, lightVB.GetAddressOf(), lightVB.GetStridePtr(), lightVB.GetOffsetPtr());
    pDeviceContext->IASetIndexBuffer(lightIB.Get(), DXGI_FORMAT_R32_UINT, 0u);
    /*if (lightIB is empty)
    {
        pDeviceContext->Draw(lightVB.GetBufferSize(), 0u);  
    }
    else*/
        pDeviceContext->DrawIndexed(lightIB.GetBufferSize(), 0u, 0);
}

void Light::SetAmbientColor(float x, float y, float z, float w)
{
    if (!LightParams) return;
    LightParams->ambient = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 Light::GetAmbientColor()const
{
    return LightParams ? LightParams->ambient : XMFLOAT4{};
}

void Light::SetDiffuseColor(float x, float y, float z, float w)
{
    if (!LightParams) return;
    LightParams->diffuse = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 Light::GetDiffuseColor()const
{
    return LightParams ? LightParams->diffuse : XMFLOAT4{};
}

void Light::SetSpecularColor(float x, float y, float z, float w)
{
    if (!LightParams) return;
    LightParams->specular = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 Light::GetSpecularColor()const
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

XMFLOAT3 Light::GetDirection()const
{
    return LightParams ? LightParams->direction : XMFLOAT3{};
}

void Light::SetRange(const float radius)
{
    if (!LightParams) return;
    LightParams->range = radius;
}

float Light::GetRange() const
{
    return LightParams ? LightParams->range : 0.0f;
}

void Light::SetAttenuation(float x, float y, float z)
{
    if (!LightParams) return;
    LightParams->attenuation = XMFLOAT3(x, y, z);
}

XMFLOAT3 Light::GetAttenuation()const
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

/*
 * Directional Light
 */
DirectionalLight::DirectionalLight(const std::string& filePath) : Light(filePath)
{
    LightType = ELightType::Directional;
}

DirectionalLight::~DirectionalLight() noexcept
{

}

void DirectionalLight::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
    SetLookAt(LightParams->direction.x, LightParams->direction.y, LightParams->direction.z);
    Light::Init(pDevice, pDeviceContext, filePath, L"./Data/Textures/brick.png");
}

void DirectionalLight::Update(const ScaldTimer& st)
{
    Light::Update(st);
    SetLookAt(LightParams->direction.x, LightParams->direction.y, LightParams->direction.z);
}

void DirectionalLight::Draw()
{
    Light::Draw();
}

/*
 * Point Light
 */
PointLight::PointLight(const std::string& filePath) : Light(filePath)
{
    LightType = ELightType::Point;
}

PointLight::~PointLight() noexcept
{
}

void PointLight::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
    Light::Init(pDevice, pDeviceContext, filePath, L"./Data/Textures/test_texture.png");
}

void PointLight::Update(const ScaldTimer& st)
{
    Light::Update(st);
}

void PointLight::Draw()
{
    SceneGeometry::Draw();
}

void PointLight::UpdateLightParams()
{
    if (!LightParams) return;

    XMFLOAT3 tmp;
    XMStoreFloat3(&tmp, GetPosition());
    LightParams->position = tmp;

    LightParams->range = CalcPointLightRange();
}

float PointLight::CalcPointLightRange()
{
    const auto color = LightParams->diffuse;
    const auto attenuation = LightParams->attenuation;

    float MaxChannel = fmax(fmax(color.x, color.y), color.z);

    float ret = (-attenuation.y + sqrtf(attenuation.y * attenuation.y - 4 * attenuation.z * (attenuation.z - 256 * MaxChannel * color.w))) / (2 * attenuation.z);
    return ret;
}

/*
 * Spot Light
 */
SpotLight::SpotLight(const std::string& filePath) : Light(filePath)
{
    LightType = ELightType::Spot;
}

SpotLight::~SpotLight() noexcept
{
}

void SpotLight::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
    Light::Init(pDevice, pDeviceContext, filePath, L"./Data/Textures/brick.png");
}

void SpotLight::Update(const ScaldTimer& st)
{
    Light::Update(st);
}

void SpotLight::Draw()
{
    Light::Draw();
}