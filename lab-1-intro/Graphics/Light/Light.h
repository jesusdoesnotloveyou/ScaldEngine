#pragma once

#include "LightHelper.h"
#include "../../Objects/Geometry/SceneGeometry.h"

class Light : public SceneGeometry
{
public:
	Light(const std::string& filePath = "");
	virtual ~Light() noexcept override;

	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"");
	virtual void Update(const ScaldTimer& st) override;
	virtual void Draw(const XMMATRIX& viewProjectionMatrix);
	//~ End of SceneGeometry interface

protected:
	XMFLOAT3 mLightParamsPos;

private:
	std::string modelPath;
};