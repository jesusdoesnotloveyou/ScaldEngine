#pragma once

#include "SceneGeometry.h"

class ModelData;

class Actor : public SceneGeometry
{
public:
	Actor();
	Actor(ModelData* modelData);
	virtual ~Actor() noexcept override;
public:
	virtual void Update(const ScaldTimer& st);
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"");

	FORCEINLINE CollisionComponent* GetCollisionComponent()const { return mCollisionComponent; }
protected:
	CollisionComponent* mCollisionComponent = nullptr;
private:
	ModelData* mModelData = nullptr;
};