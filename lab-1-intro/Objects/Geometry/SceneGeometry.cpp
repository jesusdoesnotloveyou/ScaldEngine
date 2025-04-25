#include "../../ScaldException.h"
#include "SceneGeometry.h"

// could be used to set materials and models
SceneGeometry::SceneGeometry()
{
    mCollisionComponent = new CollisionComponent(this);
    //mRenderComponent = new RenderComponent{};
}

SceneGeometry::SceneGeometry(const tuple<vector<VertexTex>, vector<DWORD>>& vi)
{}

SceneGeometry::~SceneGeometry()
{
    if (mCollisionComponent) delete mCollisionComponent;
    //if (mRenderComponent) delete mRenderComponent;
}

void SceneGeometry::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
    if (!model.Init(pDevice, pDeviceContext, filePath, texturePath))
    {
        throw std::exception{};
    }
}

void SceneGeometry::Update(const ScaldTimer& st)
{
    SceneComponent::Update(st);
    mCollisionComponent->Update(st);
    UpdateObjectCBs(st);
}

void SceneGeometry::Draw(const XMMATRIX& viewProjectionMatrix)
{
    ConstBufferVS bufferVS = {};
    bufferVS.gWorldViewProj = XMMatrixTranspose(GetTransform()->mWorldMatrix * viewProjectionMatrix);
    bufferVS.gWorld = XMMatrixTranspose(GetTransform()->mWorldMatrix);

    ConstBufferPS bufferPS = {};
    bufferPS.ambientLightColor = { 1.0f, 1.0f, 1.0f };
    bufferPS.ambientLightStrength = 0.9f;

    // SEngine->GetLight() should be
    // HARDCODE: see light transform in Engine.cpp
    bufferPS.dynamicLightColor = { 1.0f, 1.0f, 1.0f };
    bufferPS.dynamicLightStrength = 5.0f;
    bufferPS.dynamicLightPosition = { 0.0f, 4.0f, 2.0f };
    //
    // for attenuation default values are used

    model.GetConstantBufferVS().SetData(bufferVS);
    model.GetConstantBufferPS().SetData(bufferPS);
    
    model.Draw();
}

void SceneGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    model.GetConstantBufferVS().ApplyChanges();
    model.GetConstantBufferPS().ApplyChanges();
}