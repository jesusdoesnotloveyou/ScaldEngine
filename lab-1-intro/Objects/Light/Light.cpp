#include "Light.h"

void Light::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{

}

void Light::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
}

void Light::Draw(const XMMATRIX& viewMatrixProjectionMatrix)
{
}
