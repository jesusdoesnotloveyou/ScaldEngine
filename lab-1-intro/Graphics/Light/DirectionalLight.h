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
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	//~ End of SceneGeometry interface

public:
	//~ Begin of Light interface
	virtual void SetAmbientColor(float x, float y, float z, float w) override;
	virtual void SetDiffuseColor(float x, float y, float z, float w) override;

	virtual XMFLOAT4 GetAmbientColor() override;
	virtual XMFLOAT4 GetDiffuseColor() override;

	virtual void SetDirection(float x, float y, float z) override;
	virtual XMFLOAT3 GetDirection() override;
	
private:
	virtual void SetAttenuation(float x, float y, float z) override {}
	virtual XMFLOAT3 GetAttenuation() override { return XMFLOAT3{}; }
	//~ End of Light interface

public:
	FORCEINLINE DirectionalLightParams* GetParams() const { return LightParams; }
	void UpdateParams(XMFLOAT4 ambientLight = { 1.0f, 1.0f, 1.0f, 0.9f }, XMFLOAT4 diffuseLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT4 specularLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT3 direction = { 0.57735f, -0.57735f, 0.57735f });

private:
	DirectionalLightParams* LightParams = nullptr;
};