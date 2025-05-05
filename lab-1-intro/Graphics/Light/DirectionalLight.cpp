#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const std::string& filePath) : Light(filePath)
{
	LightParams = new DirectionalLightParams();
}

DirectionalLight::~DirectionalLight() noexcept
{
	if (LightParams) delete LightParams;
}

void DirectionalLight::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	Light::Init(pDevice, pDeviceContext, filePath, L"./Data/Textures/brick.png");
}

void DirectionalLight::Update(const ScaldTimer& st)
{
	Light::Update(st);
}

void DirectionalLight::Draw(const XMMATRIX& viewProjectionMatrix)
{
	Light::Draw(viewProjectionMatrix);
}

void DirectionalLight::SetLightParams(XMFLOAT4 ambientLight, XMFLOAT4 diffuseLight, XMFLOAT4 specularLight, XMFLOAT3 direction)
{
	if (!LightParams) return;

	LightParams->ambient = ambientLight;
	LightParams->diffuse = diffuseLight;
	LightParams->specular = specularLight;
	LightParams->direction = direction;
}