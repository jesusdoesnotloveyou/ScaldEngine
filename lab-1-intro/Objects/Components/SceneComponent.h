#pragma once

#include "ScaldComponent.h"
#include "TransformComponent.h"
#include "../../Graphics/ScaldCoreTypes.h"

class SceneComponent : public ScaldComponent
{
public:
	SceneComponent();
	virtual ~SceneComponent() override;
	virtual void Update(const ScaldTimer& st) override;

public:
	FORCEINLINE XMVECTOR GetPosition() const
	{
		return mTransformComponent->GetPositionVector();
	}

	FORCEINLINE XMVECTOR GetRotation() const
	{
		return mTransformComponent->GetRotationVector();
	}

	FORCEINLINE XMVECTOR GetScale() const
	{
		return mTransformComponent->GetScaleVector();
	}

	virtual void SetPosition(const XMVECTOR& pos);
	virtual void SetPosition(float x, float y, float z);
	virtual void AdjustPosition(const XMVECTOR& pos);
	virtual void AdjustPosition(float x, float y, float z);

	virtual void SetRotation(const XMVECTOR& rot);
	virtual void SetRotation(float x, float y, float z);
	virtual void AdjustRotation(const XMVECTOR& rot);
	virtual void AdjustRotation(float x, float y, float z);

	void SetScale(const XMVECTOR& scale);
	void SetScale(float x, float y, float z);
	void AdjustScale(const XMVECTOR& scale);
	void AdjustScale(float x, float y, float z);

	FORCEINLINE XMVECTOR GetForwardVector()const
	{
		return mTransformComponent->GetForwardVector();
	}

	FORCEINLINE XMVECTOR GetRightVector()const
	{
		return mTransformComponent->GetRightVector();
	}

	FORCEINLINE XMVECTOR GetUpVector()const
	{
		return mTransformComponent->GetUpVector();
	}

	void SetForwardVector(const XMVECTOR& relativeForwardVector);
	void SetRightVector(const XMVECTOR& relativeRightVector);
	void SetUpVector(const XMVECTOR& relativeUpVector);

	FORCEINLINE TransformComponent* GetTransform()const { return mTransformComponent; }

private:
	TransformComponent* mTransformComponent = nullptr;
};