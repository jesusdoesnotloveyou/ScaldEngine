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

public:
	//~ Begin of Light interface
	virtual void SetDiffuseColor(float x, float y, float z, float w) override;
	virtual void SetAttenuation(float x, float y, float z) override;

	virtual XMFLOAT4 GetDiffuseColor() override;
	virtual XMFLOAT3 GetAttenuation() override;
private:
	virtual void SetDirection(float x, float y, float z) override {}
	virtual XMFLOAT3 GetDirection() override { return XMFLOAT3{}; }
	//~ End of Light interface

public:
	FORCEINLINE PointLightParams* GetParams() const { return LightParams; }
	void UpdateParams(const ScaldTimer& st);
private:
	PointLightParams* LightParams = nullptr;
};