#include "../../ScaldException.h"
#include "SceneGeometry.h"

// could be used to set materials and models
SceneGeometry::SceneGeometry()
{
    mMovementComponent = new MovementComponent{};
    //mRenderComponent = new RenderComponent{};
}

SceneGeometry::SceneGeometry(const tuple<vector<VertexTex>, vector<DWORD>>& vi)
{}

SceneGeometry::~SceneGeometry()
{
    if (mMovementComponent) delete mMovementComponent;
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
    mMovementComponent->Update(st);
    UpdateObjectCBs(st);
}

void SceneGeometry::Draw(const XMMATRIX& viewProjectionMatrix)
{
    model.GetConstantBuffer().SetData(XMMatrixTranspose(GetTransform()->mWorldMatrix * viewProjectionMatrix));
    model.Draw();
}

void SceneGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    model.GetConstantBuffer().ApplyChanges();
}