#pragma once

#include "../../Graphics/Model.h"
#include "../Components/SceneComponent.h"
#include "../Components/Movement/MovementComponent.h"
#include "../Components/Collision/CollisionComponent.h"

#include <cmath>
#include <vector>
#include <tuple>

using std::vector;
using std::tuple;

class SceneGeometry : public SceneComponent
{
public:
	SceneGeometry();
	SceneGeometry(const tuple<vector<VertexTex>, vector<DWORD>>& vi);
	// would be changed to normal constructor
	virtual ~SceneGeometry() noexcept override;

	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") = 0;
	virtual void Update(const ScaldTimer& st) = 0;
	virtual void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);
protected:
	// @todo: From Luna's book
	void UpdateObjectCBs(const ScaldTimer& st);

public:
	FORCEINLINE CollisionComponent* GetCollisionComponent()const	{ return mCollisionComponent; }
	//FORCEINLINE RenderComponent*	GetRenderComponent()const		{ return mRenderComponent; }
	//FORCEINLINE InputComponent*	GetInputComponent()const		{ return mInputComponent; }
public:
	std::string ObjectName = "";
protected:
	Model model;

	/*std::vector<VertexTex> vertices;
	std::vector<DWORD> indices;*/

	CollisionComponent* mCollisionComponent = nullptr;
	//RenderComponent*	mRenderComponent	= nullptr;
	//InputComponent*	mInputComponent		= nullptr;
};