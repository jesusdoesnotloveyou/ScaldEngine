#include "SpotLight.h"

SpotLight::SpotLight(const std::string& filePath) : Light(filePath)
{
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