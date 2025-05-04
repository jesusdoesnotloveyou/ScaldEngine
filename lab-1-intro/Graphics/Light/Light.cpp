#include "Light.h"

Light::Light(const std::string& filePath)
{
	modelPath = filePath;
	LightParams = new PointLight();
}

Light::~Light() noexcept
{
	if (LightParams) delete LightParams;
}

void Light::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	SceneGeometry::Init(pDevice, pDeviceContext, modelPath, L"./Data/Textures/test_texture.png");
}

void Light::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
	SetPointLightParams(LightParams->ambient, LightParams->diffuse, LightParams->specular, LightParams->attenuation);
}

void Light::Draw(const XMMATRIX& viewMatrixProjectionMatrix)
{
	SceneGeometry::Draw(viewMatrixProjectionMatrix);
}

void Light::SetPointLightParams(XMFLOAT4 ambientLight, XMFLOAT4 diffuseLight, XMFLOAT4 specularLight, XMFLOAT3 attenuation)
{
	if (!LightParams) return;

	LightParams->ambient = ambientLight;
	LightParams->diffuse = diffuseLight;
	LightParams->specular = specularLight;
	LightParams->attenuation = attenuation;

	XMStoreFloat3(&mLightParamsPos, GetPosition());
	LightParams->position = mLightParamsPos;
}