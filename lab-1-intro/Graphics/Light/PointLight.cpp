#include "PointLight.h"

PointLight::PointLight(const std::string& filePath) : Light(filePath)
{
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
	Light::Update(st);
}

void PointLight::Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix)
{
	SceneGeometry::Draw(viewMatrix, projectionMatrix);
}

void PointLight::UpdateLightParams()
{
	if (!LightParams) return;

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, GetPosition());
	LightParams->position = tmp;
}