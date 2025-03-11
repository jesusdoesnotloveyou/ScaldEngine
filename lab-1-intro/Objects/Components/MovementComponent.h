#pragma once

#include "IScaldComponent.h"
#include "DirectXMath.h"

namespace dx = DirectX;

class MovementComponent : IScaldComponent
{
public:

	MovementComponent() = default;
	virtual ~MovementComponent() noexcept override = default;

	virtual void Update(float DeltaTime) override;

	float GetSpeed() const;
	void SetSpeed(dx::XMFLOAT3 newSpeed);

private:

	dx::XMFLOAT3 Velocity = { 0.0f, 0.0f, 0.0f };
};