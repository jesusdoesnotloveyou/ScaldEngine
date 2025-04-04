#pragma once

#include "SceneGeometry.h"

class Actor : public SceneGeometry
{
public:
	Actor() = default;
	virtual ~Actor() override = default;
public:
	virtual void Update(const ScaldTimer& st);
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* pTexture = nullptr, const std::string& filePeth = "");
};