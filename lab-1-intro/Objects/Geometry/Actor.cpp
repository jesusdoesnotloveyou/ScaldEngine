#include "Actor.h"
#include "../../Data/ModelData.h"

Actor::Actor()
{
	mCollisionComponent = new CollisionComponent{};
}

Actor::Actor(ModelData* modelData) : Actor()
{
	mModelData = modelData;
}

Actor::~Actor() noexcept
{
	if (mCollisionComponent) delete mCollisionComponent;
}

void Actor::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
	mCollisionComponent->Update(st);
}

void Actor::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	SceneGeometry::Init(pDevice, pDeviceContext, mModelData->modelPath, mModelData->texturePath);
}