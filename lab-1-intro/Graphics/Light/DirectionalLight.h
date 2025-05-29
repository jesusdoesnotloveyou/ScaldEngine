#pragma once

#include "Light.h"

class DirectionalLight final : public Light
{
public:
	DirectionalLight(const std::string& filePath = "");
	virtual ~DirectionalLight() noexcept override;

	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
	virtual void Update(const ScaldTimer& st) override;
	virtual void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix) override;
	//~ End of SceneGeometry interface

public:
	//~ Begin of Light interface
private:
	virtual void SetAttenuation(float x, float y, float z) override {}
	virtual XMFLOAT3 GetAttenuation()const override { return XMFLOAT3{}; }
	//~ End of Light interface
};