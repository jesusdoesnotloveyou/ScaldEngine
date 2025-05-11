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
	virtual bool IsPlayerPawn() const;
	FORCEINLINE void DisableShadowCasting() { bCastsShadow = false; }

private:
	ModelData* mModelData = nullptr;

	bool bCastsShadow = true;
};