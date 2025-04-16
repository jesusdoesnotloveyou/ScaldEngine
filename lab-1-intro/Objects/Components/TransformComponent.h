#pragma once

#include "ScaldComponent.h"

class TransformComponent : public ScaldComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent() noexcept override {}
	virtual void Update(const ScaldTimer& st) override;

	void SetWorldMatrix(const XMMATRIX& worldMat);
	void Reset();

public:
	XMVECTOR GetPositionVector() const;
	XMFLOAT3 GetPositionFloat3() const;
	XMVECTOR GetRotationVector() const;
	XMFLOAT3 GetRotationFloat3() const;
	XMVECTOR GetScaleVector() const;
	XMFLOAT3 GetScaleFloat3() const;

	void SetScale(const XMVECTOR& scaleVector);
	void SetScale(const XMFLOAT3& scale);
	void SetScale(float x, float y, float z);
	void AdjustScale(const XMVECTOR& scaleVector);
	void AdjustScale(const XMFLOAT3& scale);
	void AdjustScale(float x, float y, float z);

	void SetRotation(const XMVECTOR& rotVector);
	void SetRotation(const XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rotVector);
	void AdjustRotation(const XMFLOAT3& rot);
	void AdjustRotation(float x, float y, float z);

	void SetPosition(const XMVECTOR& posVector);
	void SetPosition(const XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& posVector);
	void AdjustPosition(const XMFLOAT3& pos);
	void AdjustPosition(float x, float y, float z);

	XMVECTOR GetForwardVector()const;
	XMVECTOR GetRightVector()const;
	XMVECTOR GetUpVector()const;

	void SetForwardVector(const XMVECTOR& ForwardVector);
	void SetRightVector(const XMVECTOR& RightVector);
	void SetUpVector(const XMVECTOR& UpVector);

	void SetParentTransform(TransformComponent* parentTransform);
	void UpdateWorldMatrix();
private:
	TransformComponent* mParentTransform = nullptr;

	// Need to init
	XMFLOAT3 mScale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 mRot	= { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 mPos	= { 0.0f, 0.0f, 0.0f };
	XMVECTOR mScaleVector;
	XMVECTOR mRotVector;
	XMVECTOR mPosVector;

	// local object's orientation vectors
	XMVECTOR mForwardVector = ScaldMath::ForwardVector;
	XMVECTOR mRightVector = ScaldMath::RightVector;
	XMVECTOR mUpVector = ScaldMath::UpVector;
public:
	XMMATRIX mScaleMatrix;
	XMMATRIX mRotationMatrix;
	XMMATRIX mTranslationMatrix;
	XMMATRIX mWorldMatrix;
};