#pragma once

#include "IScaldComponent.h"
#include "DirectXCollision.h"

class CollisionComponent : public IScaldComponent
{
public:
	~CollisionComponent() override;
	virtual void Update(float DeltaTime) override;

	void Initialize(const STransform& ownerTransform/*DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extends*/);

	DirectX::XMFLOAT3 GetCenter() const;
	DirectX::XMFLOAT3 GetExtends() const;

	void UpdateOwnerTransform(const STransform& ownerTransform);
	void SetExtends(const DirectX::XMFLOAT3& extends);
	void SetCenter(const DirectX::XMFLOAT3& center);

private:
	DirectX::BoundingBox BoundingBox;
};