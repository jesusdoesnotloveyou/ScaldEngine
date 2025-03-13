#pragma once

#include "IScaldComponent.h"

#include "DirectXMath.h"

class TransformComponent : public IScaldComponent
{
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

public:
	virtual void  Update(float DeltaTime) override {}
};