#pragma once

#include "Graphics/Model.h"

namespace Scald
{
    class PrimitiveSceneProxy
    {
    public:
        PrimitiveSceneProxy();
        ~PrimitiveSceneProxy() noexcept;
        virtual void Draw() const;

    protected:
        // @todo: From Luna's book
        virtual void UpdateObjectCBs(float deltaTime);
        
    private:
        Model m_model;
    };
}