#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const std::string& filePath) : Light(filePath)
{
	LightParams = new DirectionalLightParams();
	LightType = ELightType::Directional;
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
	SetLookAt(LightParams->direction.x, LightParams->direction.y, LightParams->direction.z);
	Light::Update(st);
}

void DirectionalLight::Draw(const XMMATRIX& viewProjectionMatrix)
{
	Light::Draw(viewProjectionMatrix);
}

void DirectionalLight::SetAmbientColor(float x, float y, float z, float w)
{
	if (!LightParams) return;
	LightParams->ambient = XMFLOAT4(x, y, z, w);
}

void DirectionalLight::SetDiffuseColor(float x, float y, float z, float w)
{
	if (!LightParams) return;
	LightParams->diffuse = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 DirectionalLight::GetAmbientColor()
{
	return LightParams ? LightParams->ambient : XMFLOAT4{};
}

XMFLOAT4 DirectionalLight::GetDiffuseColor()
{
	return LightParams ? LightParams->diffuse : XMFLOAT4{};
}

void DirectionalLight::SetDirection(float x, float y, float z)
{
	if (!LightParams) return;
	LightParams->direction = XMFLOAT3(x, y, z);
}

XMFLOAT3 DirectionalLight::GetDirection()
{
	return LightParams ? LightParams->direction : XMFLOAT3{};
}

void DirectionalLight::UpdateParams(XMFLOAT4 ambientLight, XMFLOAT4 diffuseLight, XMFLOAT4 specularLight, XMFLOAT3 direction)
{
	if (!LightParams) return;

	LightParams->ambient = ambientLight;
	LightParams->diffuse = diffuseLight;
	LightParams->specular = specularLight;
	LightParams->direction = direction;
}