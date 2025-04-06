#include "Actor.h"
#include "../../Data/ModelData.h"

Actor::Actor(ModelData* modelData)
{
	mModelData = modelData;
}

void Actor::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
}

void Actor::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	SceneGeometry::Init(pDevice, pDeviceContext, mModelData->modelPath, mModelData->texturePath);
}