#pragma once

class ScaldObject
{
public:
    ScaldObject() = default;
    // it is important to make body for destructor
    virtual ~ScaldObject() noexcept {};
};