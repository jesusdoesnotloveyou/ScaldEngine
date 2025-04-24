#include "Light.h"

Light::Light(const std::string& filePath)
{
	modelPath = filePath;
}

void Light::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	SceneGeometry::Init(pDevice, pDeviceContext, modelPath, L"./Data/Textures/test_texture.png");
}

void Light::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
}

void Light::Draw(const XMMATRIX& viewMatrixProjectionMatrix)
{
	SceneGeometry::Draw(viewMatrixProjectionMatrix);
}