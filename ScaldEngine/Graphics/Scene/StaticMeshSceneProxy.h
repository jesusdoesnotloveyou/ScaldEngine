#pragma once

namespace Scald
{
    class StaticMeshSceneProxy : public PrimitiveSceneProxy
    {
    public:
        StaticMeshSceneProxy() = default;
        virtual ~StaticMeshSceneProxy() noexcept override = default;
        
    private:

        // const StaticMesh* m_staticMesh = nullptr;
    };
}