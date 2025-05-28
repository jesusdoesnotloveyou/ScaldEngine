#include "DirectionalLight.h"

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

void DirectionalLight::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	Light::Draw(viewMatrix, projectionMatrix);
}