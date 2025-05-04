#pragma once

#include "LightHelper.h"
#include "../../Objects/Geometry/SceneGeometry.h"

enum ELightType : uint8_t
{
	NONE = 0,
	Point,
	Directional,
	Spot,
	MAX = 4
};

class Light : public SceneGeometry
{
public:
	Light(const std::string& filePath = "");
	virtual ~Light() noexcept override;

	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"");
	virtual void Update(const ScaldTimer& st) override;
	virtual void Draw(const XMMATRIX& viewMatrixProjectionMatrix);
	
	FORCEINLINE PointLight* GetPointLightParams() const { return LightParams; }
	void SetPointLightParams(XMFLOAT4 ambientLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT4 diffuseLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT4 specularLight = { 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT3 attenuation = { 1.0f, 0.0f, 0.0f });

	XMFLOAT3 lightColor = XMFLOAT3{ 1.0f, 1.0f, 1.0f };
	float lightStrength = 10.0f;

	// To avoid division by zero
	float attenuationConstantFactor = 1.0f;
	// The effect decreases linearly with distance from the light source
	float attenuationLinearFactor = 0.1f;
	// Soft effect
	float attenuationExponentialFactor = 0.1f;
private:
	PointLight* LightParams = nullptr;
	ELightType LightType;
	XMFLOAT3 mLightParamsPos;

	std::string modelPath;
};