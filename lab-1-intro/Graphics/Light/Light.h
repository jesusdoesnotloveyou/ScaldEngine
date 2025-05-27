#pragma once

#include "LightHelper.h"
#include "../../Objects/Geometry/SceneGeometry.h"

class Light : public SceneGeometry
{
public:
	Light(const std::string& filePath = "");
	virtual ~Light() noexcept override;

	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
	virtual void Update(const ScaldTimer& st) override;
    virtual void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix) override;
	//~ End of SceneGeometry interface
	
    // 4th parameter is intensity
    virtual void SetAmbientColor(float x, float y, float z, float w); // only for dir light in Deferred Shading
    virtual XMFLOAT4 GetAmbientColor(); // only for dir light in Deferred Shading
    virtual void SetDiffuseColor(float x, float y, float z, float w);
    virtual XMFLOAT4 GetDiffuseColor();
    virtual void SetSpecularColor(float x, float y, float z, float w);
    virtual XMFLOAT4 GetSpecularColor();
    // only for directional and spot lights
    virtual void SetLookAt(float x, float y, float z);
    virtual void SetDirection(float x, float y, float z);
    virtual XMFLOAT3 GetDirection();
    // only for point and spot lights
    virtual void SetAttenuation(float x, float y, float z);
    virtual XMFLOAT3 GetAttenuation();

    void GenerateViewMatrix();
    void GeneratePerspectiveProjectionMatrix(float, float);
    void GenerateOrthographicProjectionMatrix(float ViewWidth, float ViewHeight, float NearZ, float FarZ);

    const XMMATRIX& GetViewMatrix() const;
    const XMMATRIX& GetPerspectiveProjectionMatrix() const;
    const XMMATRIX& GetOrthographicProjectionMatrix() const;

    FORCEINLINE ELightType GetLightType() const { return LightType; }
    FORCEINLINE LIGHT_DESC* GetLightParams()const { return LightParams; }
    virtual void UpdateLightParams();

protected:
    ELightType LightType = ELightType::None;
    LIGHT_DESC* LightParams;
private:
    XMFLOAT3 mLookAt;
    XMMATRIX mViewMatrix;
    XMMATRIX mPerspectiveProjectionMatrix;
    XMMATRIX mOrthographicProjectionMatrix;
private:
	std::string modelPath;
};