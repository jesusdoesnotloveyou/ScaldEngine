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
	
    // 4th parameter is intensity
    virtual void SetAmbientColor(float x, float y, float z, float w) = 0;
    virtual void SetDiffuseColor(float x, float y, float z, float w) = 0;
    virtual XMFLOAT4 GetAmbientColor() = 0;
    virtual XMFLOAT4 GetDiffuseColor() = 0;

    // only for directional and spot lights
    virtual void SetLookAt(float x, float y, float z);
    virtual void SetDirection(float x, float y, float z) = 0;
    virtual XMFLOAT3 GetDirection() = 0;

    // only for point and spot lights
    virtual void SetAttenuation(float x, float y, float z) = 0;
    virtual XMFLOAT3 GetAttenuation() = 0;

    void GenerateViewMatrix();
    void GeneratePerspectiveProjectionMatrix(float, float);
    void GenerateOrthographicProjectionMatrix(float ViewWidth, float ViewHeight, float NearZ, float FarZ);

    const XMMATRIX& GetViewMatrix() const;
    const XMMATRIX& GetPerspectiveProjectionMatrix() const;
    const XMMATRIX& GetOrthographicProjectionMatrix() const;

    FORCEINLINE ELightType GetLightType() const { return LightType; }

private:
    XMFLOAT3 mLookAt;
    XMMATRIX mViewMatrix;
    XMMATRIX mPerspectiveProjectionMatrix;
    XMMATRIX mOrthographicProjectionMatrix;

protected:
    ELightType LightType = ELightType::None;
private:
	std::string modelPath;
};