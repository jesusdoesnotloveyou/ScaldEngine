#include "SpotLight.h"

SpotLight::SpotLight(const std::string& filePath) : Light(filePath)
{
	LightParams = new SpotLightParams();
	LightType = ELightType::Spot;
}

SpotLight::~SpotLight() noexcept
{
	if (LightParams) delete LightParams;
}

void SpotLight::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	Light::Init(pDevice, pDeviceContext, filePath, L"./Data/Textures/brick.png");
}

void SpotLight::Update(const ScaldTimer& st)
{
	Light::Update(st);
}

void SpotLight::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	Light::Draw(viewMatrix, projectionMatrix);
}

void SpotLight::SetDiffuseColor(float x, float y, float z, float w)
{
	if (!LightParams) return;
	LightParams->diffuse = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 SpotLight::GetDiffuseColor()
{
	return LightParams ? LightParams->diffuse : XMFLOAT4{};
}

void SpotLight::SetDirection(float x, float y, float z)
{
	if (!LightParams) return;
	LightParams->direction = XMFLOAT3(x, y, z);
}

XMFLOAT3 SpotLight::GetDirection()
{
	return LightParams ? LightParams->direction : XMFLOAT3{};
}

void SpotLight::SetAttenuation(float x, float y, float z)
{
	if (!LightParams) return;
	LightParams->attenuation = XMFLOAT3(x, y, z);
}

XMFLOAT3 SpotLight::GetAttenuation()
{
	return LightParams ? LightParams->attenuation : XMFLOAT3{};
}