#pragma once

#include "ScaldCoreTypes.h"
#include "../Objects/Components/SceneComponent.h"

class Camera : public SceneComponent
{
public:
	Camera();
	virtual ~Camera() override = default;
	virtual void Update(const ScaldTimer& st) override;

	virtual void SetPosition(const XMVECTOR& pos) override;
	virtual void SetPosition(float x, float y, float z) override;
	virtual void AdjustPosition(const XMVECTOR& pos) override;
	virtual void AdjustPosition(float x, float y, float z) override;

	virtual void SetRotation(const XMVECTOR& rot) override;
	virtual void SetRotation(float x, float y, float z) override;
	virtual void AdjustRotation(const XMVECTOR& rot) override;
	virtual void AdjustRotation(float x, float y, float z) override;

	//~ Begin of Camera specific
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	void SetOrthographicProjectionValues(float width, float height, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;
	const XMMATRIX& GetOrthographicProjectionMatrix() const;
	
	void SetLookAtPosition(XMFLOAT3 lookAtPosition);
	void SetLookAtPosition(XMVECTOR lookAtPosition);
	// camera orbital rotation specific
	void SetupAttachment(TransformComponent* transformToAttach);
	void ClearAttachment();
	FORCEINLINE bool IsAttached() const { return bIsAttached; }
protected:
	virtual void UpdateViewMatrix();
	XMMATRIX mViewMatrix;
	XMMATRIX mProjectionMatrix;
	XMMATRIX mOrthographicMatrix;

	float mSpeed = 0.0f;
	bool bIsAttached = false;
	//~ End of Camera specific
};