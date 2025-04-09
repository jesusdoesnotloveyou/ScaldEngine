#pragma once

#include "../../Graphics/Model.h"
#include "../Components/TransformComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/Collision/CollisionComponent.h"
#include "../Components/InputComponent.h"
#include "../Components/RenderComponent.h"

#include <cmath>
#include <vector>
#include <tuple>

using std::vector;
using std::tuple;

//class RenderComponent;
//class CollisionComponent;
//class InputComponent;
//class MovementComponent;

class SceneGeometry
{
public:
	SceneGeometry();
	SceneGeometry(const tuple<vector<VertexTex>, vector<DWORD>>& vi);
	// would be changed to normal constructor
	virtual ~SceneGeometry();

	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") = 0;
	virtual void Update(const ScaldTimer& st) = 0;
	virtual void Draw(const XMMATRIX& viewMatrixProjectionMatrix);
protected:
	// @todo: From Luna's book
	void UpdateObjectCBs(const ScaldTimer& st);

public:
	FORCEINLINE MovementComponent*	GetMovement()const				{ return mMovementComponent; }
	FORCEINLINE TransformComponent* GetTransform()const				{ return mTransformComponent; }
	FORCEINLINE CollisionComponent* GetCollisionComponent()const	{ return mCollisionComponent; }
	FORCEINLINE RenderComponent*	GetRenderComponent()const		{ return mRenderComponent; }
	FORCEINLINE InputComponent*		GetInputComponent()const		{ return mInputComponent; }

	XMVECTOR GetForwardVector()const;
	XMVECTOR GetRightVector()const;
	XMVECTOR GetUpVector() const;

protected:
	Model model;

	std::vector<VertexTex> vertices;
	std::vector<DWORD> indices;

	TransformComponent* mTransformComponent = nullptr;
	RenderComponent*	mRenderComponent	= nullptr;
	CollisionComponent* mCollisionComponent = nullptr;
	InputComponent*		mInputComponent		= nullptr;
	MovementComponent*	mMovementComponent	= nullptr;
};