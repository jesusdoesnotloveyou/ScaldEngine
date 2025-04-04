#include "Actor.h"

void Actor::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
}

void Actor::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* pTexture, const std::string& filePeth)
{
	SceneGeometry::Init(pDevice, pDeviceContext, pTexture, filePeth);
}