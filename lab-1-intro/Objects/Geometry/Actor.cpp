#include "stdafx.h"
#include "Actor.h"
#include "Data/ModelData.h"

Actor::Actor()
{

}

Actor::Actor(ModelData* modelData)
	: Actor()
{
	m_modelData = modelData;
}

Actor::~Actor() noexcept
{
	if (m_modelData) delete m_modelData;
}

void Actor::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
}

void Actor::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	SceneGeometry::Init(pDevice, pDeviceContext, m_modelData->modelPath, m_modelData->texturePath);
	m_modelData = nullptr;
}

bool Actor::IsPlayerPawn() const
{
	return false;
}