#include "PointLight.h"

PointLight::PointLight(const std::string& filePath) : Light(filePath)
{
	LightParams = new PointLightParams();
	LightType = ELightType::Point;
}

PointLight::~PointLight() noexcept
{
	if (LightParams) delete LightParams;
}

void PointLight::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	Light::Init(pDevice, pDeviceContext, filePath, L"./Data/Textures/test_texture.png");
}

void PointLight::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
	UpdateParams(st);
}

void PointLight::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	SceneGeometry::Draw(viewMatrix, projectionMatrix);
}

void PointLight::SetDiffuseColor(float x, float y, float z, float w)
{
	if (!LightParams) return;
	LightParams->diffuse = XMFLOAT4(x, y, z, w);
}

void PointLight::SetAttenuation(float x, float y, float z)
{
	if (!LightParams) return;
	LightParams->attenuation = XMFLOAT3(x, y, z);
}

XMFLOAT4 PointLight::GetDiffuseColor()
{
	return LightParams ? LightParams->diffuse : XMFLOAT4{};
}

XMFLOAT3 PointLight::GetAttenuation()
{
	return LightParams ? LightParams->attenuation : XMFLOAT3{};
}

void PointLight::UpdateParams(const ScaldTimer& st)
{
	if (!LightParams) return;

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, GetPosition());
	LightParams->position = tmp;
}