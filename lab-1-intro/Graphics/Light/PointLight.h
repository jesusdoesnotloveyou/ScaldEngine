#pragma once

#include "Light.h"

class PointLight final : public Light
{
public:
	PointLight(const std::string& filePath = "");
	virtual ~PointLight() noexcept override;

	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
	virtual void Update(const ScaldTimer& st) override;
	virtual void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix) override;
	//~ End of SceneGeometry interface

	//~ Begin of Light interface
private:
	virtual void SetAmbientColor(float x, float y, float z, float w) override {};
	virtual XMFLOAT4 GetAmbientColor()const override { return XMFLOAT4(); }
	virtual void SetDirection(float x, float y, float z) override {};
	virtual XMFLOAT3 GetDirection()const override { return XMFLOAT3(); }
	//~ End of Light interface

public:
	virtual void UpdateLightParams() override;
};