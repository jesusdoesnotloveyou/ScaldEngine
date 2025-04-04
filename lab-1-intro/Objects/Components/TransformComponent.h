#pragma once

#include "IScaldComponent.h"

class TransformComponent : public IScaldComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent() noexcept override {}
	virtual void Update(const ScaldTimer& st) override;

	void SetWorldMatrix(const XMMATRIX& worldMat);
	void Reset();

public:
	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	void SetScale(const XMVECTOR& scale);
	void SetScale(const XMFLOAT3& scale);
	void SetScale(float x, float y, float z);

	void SetRotation(const XMVECTOR& rot);
	void SetRotation(const XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(const XMFLOAT3& rot);
	void AdjustRotation(float x, float y, float z);

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(const XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(const XMFLOAT3& pos);
	void AdjustPosition(float x, float y, float z);

	void SetLookAtPosition(XMFLOAT3 lookAtPosition);
	void SetLookAtPosition(XMVECTOR lookAtPosition);

	XMVECTOR GetForwardVector()const;
	XMVECTOR GetRightVector()const;
	XMVECTOR GetBackVector()const;
	XMVECTOR GetLeftVector()const;

	void SetParentTransform(TransformComponent* parentTransform);
private:
	void UpdateLocalMatrix();
	void UpdateWorldMatrix();
	// Object local frame
private:
	TransformComponent* mParentTransform = nullptr;

	XMFLOAT3 mScale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 mRot	= { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 mPos	= { 0.0f, 0.0f, 0.0f };
	XMVECTOR mScaleVector;
	XMVECTOR mRotVector;
	XMVECTOR mPosVector;
	// Need to init
	// local object's orientation vectors
	XMVECTOR mForwardVector;
	XMVECTOR mLeftVector;
	XMVECTOR mRightVector;
	XMVECTOR mBackVector;
public:
	XMMATRIX mLocalMatrix;
	XMMATRIX mWorldMatrix;

	// Solar System specific
	float mOrbitRot;
};