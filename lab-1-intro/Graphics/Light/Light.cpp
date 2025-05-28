#include "Light.h"
#include "../../Objects/Geometry/3D/Shapes.h"

Light::Light(const std::string& filePath)
    :
    mLookAt(0.0f, 0.0f, 0.0f),
    mViewMatrix(XMMatrixIdentity()),
    mPerspectiveProjectionMatrix(XMMatrixIdentity()),
    mOrthographicProjectionMatrix(XMMatrixIdentity())
{
    LightParams = new LIGHT_DESC{};
	modelPath = filePath;
    mCollisionComponent->DisableCollision();
}

Light::~Light() noexcept
{
    if (LightParams) delete LightParams;
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
        std::vector<DWORD> volumeIndeces;
        Shapes::GetSphereShape(volumeVertices, volumeIndeces, 1.0f /*hard - coded value just for now*/, 8, 16);

        LightVolume = new Mesh(pDevice, pDeviceContext, volumeVertices, volumeIndeces);
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

void Light::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	SceneGeometry::Draw(viewMatrix, projectionMatrix);
}

void Light::DrawLightVolume(ID3D11DeviceContext* pDeviceContext)
{
    pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pDeviceContext->IASetVertexBuffers(0u, 1u, LightVolume->GetVertexBuffer().GetAddressOf(), LightVolume->GetVertexBuffer().GetStridePtr(), LightVolume->GetVertexBuffer().GetOffsetPtr());
    pDeviceContext->IASetIndexBuffer(LightVolume->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
    pDeviceContext->DrawIndexed(LightVolume->GetIndexBuffer().GetBufferSize(), 0u, 0);
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