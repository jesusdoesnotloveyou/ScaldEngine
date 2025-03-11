#pragma once

#include "IScaldComponent.h"
#include "DirectXCollision.h"

class CollisionComponent : public IScaldComponent
{
public:
	~CollisionComponent() override;
	virtual void Update(float DeltaTime) override;

	void Initialize(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extends);

	DirectX::XMFLOAT3 GetCenter() const;
	DirectX::XMFLOAT3 GetExtends() const;

private:
	DirectX::BoundingBox BoundingBox;
};