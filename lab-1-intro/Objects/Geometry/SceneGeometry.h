#pragma once

#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/IndexBuffer.h"
#include "../../Graphics/ConstantBuffer.h"

#include "../../ScaldCore/Engine/ScaldTimer.h"

#include "../Components/TransformComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/InputComponent.h"
#include "../Components/MovementComponent.h"

#include <cmath>
#include <vector>
#include <tuple>

using std::vector;
using std::tuple;

class RenderComponent;
class CollisionComponent;
class InputComponent;
class MovementComponent;

class SceneGeometry
{
public:
	SceneGeometry();
	SceneGeometry(const tuple<vector<VertexTex>, vector<DWORD>>& vi);
	// would be changed to normal constructor
	virtual ~SceneGeometry();

	virtual void Update(const ScaldTimer& st) = 0;
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* pTexture = nullptr) = 0;	
	virtual void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);
protected:
	// @todo: From Luna's book
	void UpdateObjectCBs(const ScaldTimer& st);

	// probably should be in movement component
private:
	void UpdateOrbitRotation(const ScaldTimer& st);
	void UpdateRotation(const ScaldTimer& st);

public:
	VertexBuffer<VertexTex>& GetVertexBuffer();
	IndexBuffer& GetIndexBuffer();
	ConstantBuffer<ConstBufferVS>& GetConstantBuffer();

	FORCEINLINE TransformComponent* GetTransform() const			{ return mTransformComponent; }
	FORCEINLINE RenderComponent* GetRenderComponent() const			{ return mRenderComponent; }
	FORCEINLINE CollisionComponent* GetCollisionComponent() const	{ return mCollisionComponent; }
	FORCEINLINE InputComponent* GetInputComponent() const			{ return mInputComponent; }
	FORCEINLINE MovementComponent* GetMovement() const				{ return mMovementComponent; }

	XMVECTOR GetForwardVector()const;
	XMVECTOR GetRightVector()const;
	XMVECTOR GetBackVector()const;
	XMVECTOR GetLeftVector()const;

protected:
	TransformComponent* mTransformComponent = nullptr;
	RenderComponent*	mRenderComponent	= nullptr;
	CollisionComponent* mCollisionComponent = nullptr;
	InputComponent*		mInputComponent		= nullptr;
	MovementComponent*	mMovementComponent	= nullptr;

	// std::vector<T>
	std::vector<VertexTex> vertices;
	std::vector<DWORD> indeces;

	ConstantBuffer<ConstBufferVS> mCB;
	VertexBuffer<VertexTex> mVB;
	IndexBuffer mIB;
};