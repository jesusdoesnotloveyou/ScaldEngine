#pragma once

#include "SceneGeometry.h"

class ModelData;

class Actor : public SceneGeometry
{
public:
	Actor() = default;
	Actor(ModelData* modelData);
	virtual ~Actor() override = default;
public:
	virtual void Update(const ScaldTimer& st);
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"");

private:
	ModelData* mModelData;
};