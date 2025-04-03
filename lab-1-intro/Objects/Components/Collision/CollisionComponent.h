#pragma once

#include "../IScaldComponent.h"
#include "DirectXCollision.h"

class CollisionComponent : public IScaldComponent
{
public:
	CollisionComponent() = default;
	virtual ~CollisionComponent() override {}
	virtual void Update(const ScaldTimer& st) override {}

	DirectX::XMFLOAT3 GetCenter() const;
	DirectX::XMFLOAT3 GetExtends() const;

	void SetExtends(const DirectX::XMFLOAT3& extends);
	void SetCenter(const DirectX::XMFLOAT3& center);

public:
	DirectX::BoundingBox BoundingBox;
};