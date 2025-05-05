#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const std::string& filePath = "");
	virtual ~PointLight() noexcept override;

	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"");
	virtual void Update(const ScaldTimer& st) override;
	virtual void Draw(const XMMATRIX& viewProjectionMatrix);
	//~ End of SceneGeometry interface

	FORCEINLINE PointLightParams* GetLightParams() const { return LightParams; }
	void SetLightParams(XMFLOAT4 ambientLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT4 diffuseLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT4 specularLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT3 attenuation = { 1.0f, 0.0f, 0.0f });
private:
	PointLightParams* LightParams = nullptr;
};