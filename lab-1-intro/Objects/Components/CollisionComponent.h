#pragma once

#include "IScaldComponent.h"
#include "DirectXCollision.h"
#include "../../Delegates/Delegates.h"

class CollisionComponent : public IScaldComponent
{
public:
	CollisionComponent() = default;
	~CollisionComponent() override;
	virtual void Update(float DeltaTime) override;

	void Initialize(const STransform& ownerTransform/*DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extends*/);

	DirectX::XMFLOAT3 GetCenter() const;
	DirectX::XMFLOAT3 GetExtends() const;

	void UpdateOwnerTransform(const STransform& ownerTransform);
	void SetExtends(const DirectX::XMFLOAT3& extends);
	void SetCenter(const DirectX::XMFLOAT3& center);

public:
	DirectX::BoundingBox BoundingBox;
};