#pragma once

#include "ScaldCoreTypes.h"

class TransformComponent;

class Camera
{
public:
	Camera();

	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

	const XMVECTOR& GetPosition() const;
	const XMVECTOR& GetRotation() const;

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(float x, float y, float z);

	void SetRotation(const XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(float x, float y, float z);

	//~ Camera specific
	void SetLookAtPosition(XMFLOAT3 lookAtPosition);
	void SetLookAtPosition(XMVECTOR lookAtPosition);
	//~ Camera specific

	XMVECTOR GetForwardVector()const;
	XMVECTOR GetRightVector()const;
	XMVECTOR GetBackVector()const;
	XMVECTOR GetLeftVector()const;

	void SetForwardVector(const XMVECTOR& relativeForwardVector);
	void SetRightVector(const XMVECTOR& relativeRightVector);
	void SetBackVector(const XMVECTOR& relativeBackVector);
	void SetLeftVector(const XMVECTOR& relativeLeftVector);

	// camera orbital rotation specific
	void SetupAttachment(TransformComponent* transformToAttach);
	void ClearAttachment();
	FORCEINLINE bool IsAttached() const { return bIsAttached; }

	FORCEINLINE TransformComponent* GetTransform()const { return mTransformComponent; }
protected:
	void UpdateViewMatrix();
	XMMATRIX mViewMatrix;
	XMMATRIX mProjectionMatrix;

	TransformComponent* mTransformComponent = nullptr;

	//~ Camera specific
	float mSpeed = 0.0f;
	bool bIsAttached = false;
};