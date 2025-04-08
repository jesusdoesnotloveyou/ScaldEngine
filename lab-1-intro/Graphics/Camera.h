#pragma once

#include "ScaldCoreTypes.h"

using namespace DirectX;

class TransformComponent;

class Camera
{
public:
	Camera();

	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;
	FORCEINLINE TransformComponent* GetTransform()const { return mTransformComponent; }

//~ Transform component
	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(const XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(const XMFLOAT3& pos);
	void AdjustPosition(float x, float y, float z);

	void SetRotation(const XMVECTOR& rot);
	void SetRotation(const XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(const XMFLOAT3& rot);
	void AdjustRotation(float x, float y, float z);

	//~ Camera specific
	void SetLookAtPosition(XMFLOAT3 lookAtPosition);
	void SetLookAtPosition(XMVECTOR lookAtPosition);
	//~ Camera specific

	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();
//~ Transform component

	// camera orbital rotation specific
	void SetupAttachment(TransformComponent* transformToAttach);
	void ClearAttachment();
	FORCEINLINE bool IsAttached() const { return bIsAttached; }

protected:
	void UpdateViewMatrix();
	//~ Transform component
	XMFLOAT3 mPos;
	XMFLOAT3 mRot;
	XMVECTOR mPosVector;
	XMVECTOR mRotVector;
	//~ Camera specific
	XMMATRIX mViewMatrix;
	XMMATRIX mProjectionMatrix;
	//~ Transform component
	XMVECTOR mForwardVector;
	XMVECTOR mLeftVector;
	XMVECTOR mRightVector;
	XMVECTOR mBackwardVector;

	TransformComponent* mTransformComponent;

	//~ Camera specific
	float mSpeed = 0.0f;
	bool bIsAttached = false;
};