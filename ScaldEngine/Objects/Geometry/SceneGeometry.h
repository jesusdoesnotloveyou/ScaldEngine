#pragma once

#include "Graphics/Model.h"
#include "Objects/Components/SceneComponent.h"
#include "Objects/Components/Collision/CollisionComponent.h"

#include <vector>
#include <string>
#include <tuple>

namespace Scald
{
    class SceneGeometry : public SceneComponent
    {
    public:
        SceneGeometry();
        SceneGeometry(const std::tuple<std::vector<Scald::VertexPositionNormalUV>, std::vector<DWORD>>& vi);
        // would be changed to normal constructor
        virtual ~SceneGeometry() noexcept override;

        virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") = 0;
        virtual void Update(const ScaldTimer& st) = 0;
        virtual void Draw() const;

    protected:
        // @todo: From Luna's book
        void UpdateObjectCBs(const ScaldTimer& st);

    public:
        FORCEINLINE CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
        // FORCEINLINE RenderComponent*	GetRenderComponent()const		{ return mRenderComponent; }
    public:
        std::string ObjectName = "";

    protected:
        Model model;

        CollisionComponent* mCollisionComponent = nullptr;
        // RenderComponent*	mRenderComponent	= nullptr;
    };
}