#pragma once

#include "IScaldComponent.h"
#include "DirectXCollision.h"

class CollisionComponent : public IScaldComponent
{
public:
	~CollisionComponent() override;
	virtual void Update() override;

private:
	DirectX::BoundingBox BoundingBox;
};