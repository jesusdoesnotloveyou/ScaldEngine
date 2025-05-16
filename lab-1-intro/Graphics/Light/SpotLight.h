#pragma once

#include "Light.h"

class SpotLight final : public Light
{
public:
	SpotLight(const std::string& filePath = "");
	virtual ~SpotLight() noexcept override;
	
public:
	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
	virtual void Update(const ScaldTimer& st) override;
	virtual void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix) override;
	//~ End of SceneGeometry interface

public:
	//~ Begin of Light interface
	virtual void SetAmbientColor(float x, float y, float z, float w) override;
	virtual void SetDiffuseColor(float x, float y, float z, float w) override;
	virtual XMFLOAT4 GetAmbientColor() override;
	virtual XMFLOAT4 GetDiffuseColor() override;

	virtual void SetDirection(float x, float y, float z) override;
	virtual XMFLOAT3 GetDirection() override;

	virtual void SetAttenuation(float x, float y, float z) override;
	virtual XMFLOAT3 GetAttenuation() override;
	//~ End of Light interface

public:
	FORCEINLINE SpotLightParams* GetParams() const { return LightParams; }

private:
	SpotLightParams* LightParams = nullptr;
};