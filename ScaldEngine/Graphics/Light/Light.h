#pragma once

#include "LightHelper.h"
#include "Objects/Geometry/SceneGeometry.h"

class Light : public SceneGeometry
{
public:
    Light(const std::string& filePath = "");
    virtual ~Light() noexcept override;

    //~ Begin of SceneGeometry interface
    virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
    virtual void Update(const ScaldTimer& st) override;
    virtual void Draw() override;
    void DrawLightVolume(ID3D11DeviceContext* pDeviceContext);
    //~ End of SceneGeometry interface

    // 4th parameter is intensity
    virtual void SetAmbientColor(float x, float y, float z, float w);  // only for dir light in Deferred Shading
    virtual XMFLOAT4 GetAmbientColor() const;                          // only for dir light in Deferred Shading
    virtual void SetDiffuseColor(float x, float y, float z, float w);
    virtual XMFLOAT4 GetDiffuseColor() const;
    virtual void SetSpecularColor(float x, float y, float z, float w);
    virtual XMFLOAT4 GetSpecularColor() const;
    // only for directional and spot lights
    virtual void SetLookAt(float x, float y, float z);
    virtual void SetDirection(float x, float y, float z);
    virtual XMFLOAT3 GetDirection() const;
    // only for point and spot lights
    virtual void SetRange(const float radius = 1.0f);
    virtual float GetRange() const;
    virtual void SetAttenuation(float x, float y, float z);
    virtual XMFLOAT3 GetAttenuation() const;

    void GenerateViewMatrix();
    void GeneratePerspectiveProjectionMatrix(float, float);
    void GenerateOrthographicProjectionMatrix(float ViewWidth, float ViewHeight, float NearZ, float FarZ);

    const XMMATRIX& GetViewMatrix() const;
    const XMMATRIX& GetPerspectiveProjectionMatrix() const;
    const XMMATRIX& GetOrthographicProjectionMatrix() const;

    FORCEINLINE ELightType GetLightType() const { return LightType; }
    // to update dynamic properties of light objects (position, direction, intensity etc.)
    virtual void UpdateLightParams();

protected:
    std::unique_ptr<LIGHT_DESC> LightParams = nullptr;
    std::unique_ptr<Mesh> LightVolume = nullptr;
    ELightType LightType = ELightType::None;

private:
    XMFLOAT3 mLookAt;
    XMMATRIX mViewMatrix;
    XMMATRIX mPerspectiveProjectionMatrix;
    XMMATRIX mOrthographicProjectionMatrix;

private:
    std::string modelPath;
};

class DirectionalLight final : public Light
{
public:
    DirectionalLight(const std::string& filePath = "");
    virtual ~DirectionalLight() noexcept override;

    //~ Begin of SceneGeometry interface
    virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
    virtual void Update(const ScaldTimer& st) override;
    virtual void Draw() override;
    //~ End of SceneGeometry interface

public:
    //~ Begin of Light interface
private:
    virtual void SetAttenuation(float x, float y, float z) override {}
    virtual XMFLOAT3 GetAttenuation() const override { return XMFLOAT3{}; }
    //~ End of Light interface
};

class PointLight final : public Light
{
public:
    PointLight(const std::string& filePath = "");
    virtual ~PointLight() noexcept override;

    //~ Begin of SceneGeometry interface
    virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
    virtual void Update(const ScaldTimer& st) override;
    virtual void Draw() override;
    //~ End of SceneGeometry interface

    //~ Begin of Light interface
private:
    virtual void SetAmbientColor(float x, float y, float z, float w) override {};
    virtual XMFLOAT4 GetAmbientColor() const override { return XMFLOAT4(); }
    virtual void SetDirection(float x, float y, float z) override {};
    virtual XMFLOAT3 GetDirection() const override { return XMFLOAT3(); }
    //~ End of Light interface

public:
    virtual void UpdateLightParams() override;

private:
    float CalcPointLightRange();
};

class SpotLight final : public Light
{
public:
    SpotLight(const std::string& filePath = "");
    virtual ~SpotLight() noexcept override;

public:
    //~ Begin of SceneGeometry interface
    virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
    virtual void Update(const ScaldTimer& st) override;
    virtual void Draw() override;
    //~ End of SceneGeometry interface

    //~ Begin of Light interface
private:
    virtual void SetAmbientColor(float x, float y, float z, float w) override {};
    virtual XMFLOAT4 GetAmbientColor() const override { return XMFLOAT4(); }
    //~ End of Light interface
};