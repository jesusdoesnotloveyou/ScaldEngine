#pragma once

#include "../Geometry/SceneGeometry.h"

class Light : public SceneGeometry
{
public:
	Light(const std::string& filePath = "");

	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"");
	virtual void Update(const ScaldTimer& st);
	virtual void Draw(const XMMATRIX& viewMatrixProjectionMatrix);
	
	XMFLOAT3 lightColor = XMFLOAT3{ 1.0f, 1.0f, 1.0f };
	float lightStrength = 10.0f;

private:
	std::string modelPath;
};