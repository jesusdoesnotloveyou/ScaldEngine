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

	XMFLOAT3 GetVelocity() const;

	XMFLOAT3 GetInitialTransition() const;
	XMFLOAT3 GetInitialVelocity() const;
	void SetVelocity(const dx::XMFLOAT3&);
	void SetVelocity(float x, float y, float z);

private:

	XMFLOAT3 InitialVelocity = { 0.01f, 0.003f, 0.0f };
	XMFLOAT3 Velocity = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 InitialTransition = { 0.0f, 0.0f, 0.0f };
};