#include "PointLight.h"

PointLight::PointLight(const std::string& filePath) : Light(filePath)
{
	LightParams = new PointLightParams();
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
	SetLightParams(LightParams->ambient, LightParams->diffuse, LightParams->specular, LightParams->attenuation);
}

void PointLight::Draw(const XMMATRIX& viewProjectionMatrix)
{
	SceneGeometry::Draw(viewProjectionMatrix);
}

void PointLight::SetLightParams(XMFLOAT4 ambientLight, XMFLOAT4 diffuseLight, XMFLOAT4 specularLight, XMFLOAT3 attenuation)
{
	if (!LightParams) return;

	LightParams->ambient = ambientLight;
	LightParams->diffuse = diffuseLight;
	LightParams->specular = specularLight;
	LightParams->attenuation = attenuation;

	XMStoreFloat3(&mLightParamsPos, GetPosition());
	LightParams->position = mLightParamsPos;
}