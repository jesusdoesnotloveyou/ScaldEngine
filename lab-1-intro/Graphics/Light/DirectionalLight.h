#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const std::string& filePath = "");
	virtual ~DirectionalLight() noexcept override;

	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"");
	virtual void Update(const ScaldTimer& st) override;
	virtual void Draw(const XMMATRIX& viewProjectionMatrix);
	//~ End of SceneGeometry interface

	FORCEINLINE DirectionalLightParams* GetLightParams() const { return LightParams; }
	void SetLightParams(XMFLOAT4 ambientLight = { 1.0f, 1.0f, 1.0f, 0.9f }, XMFLOAT4 diffuseLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT4 specularLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT3 direction = { 0.57735f, -0.57735f, 0.57735f });

private:
	DirectionalLightParams* LightParams = nullptr;
};