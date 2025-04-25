#pragma once

#include "../../Objects/Components/Movement/MovementComponent.h"

class SceneGeometry;

class KatamariMovementComponent : public MovementComponent
{
public:
	KatamariMovementComponent(SceneGeometry* OwnerPlayer);
	virtual ~KatamariMovementComponent() noexcept override = default;

	virtual void Update(const ScaldTimer& st) override;

private:
	KatamariMovementComponent(const KatamariMovementComponent&) = default;
	KatamariMovementComponent& operator=(const KatamariMovementComponent&) = default;

public:
	void OnKeyPressed(unsigned char key);
	void OnKeyReleased(unsigned char key);
private:
	void UpdateMovement(float deltaTime);
	void UpdateRotation(float deltaTime);
	XMVECTOR GetMovementDirection() const;
	XMVECTOR GetPerpendicular(XMVECTOR&& vec) const;

private:
	const unsigned char forwardKey = 'W';
	const unsigned char backKey = 'S';
	const unsigned char rightKey = 'D';
	const unsigned char leftKey = 'A';
};