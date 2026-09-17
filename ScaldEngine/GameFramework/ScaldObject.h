#pragma once

#include <string>
#include <cstdint>

namespace Scald
{
    class ScaldObject
    {
    public:
        ScaldObject() = default;
        virtual ~ScaldObject() noexcept {};
        
        const std::string& GetName() const { return m_name; }
        uint32_t GetID() const { return m_id; }

        // Begin of ScaldObject interface
        virtual void Tick(float deltaTime) = 0 {}
        // End of ScaldObject interface
    protected:
        std::string m_name;
        uint32_t m_id;
    };
}