#pragma once

#include "../Objects/Geometry/SceneGeometry.h"

class Actor : public SceneGeometry
{
public:
	Actor();
	Actor(const tuple<vector<VertexTex>, vector<DWORD>>& vi);
	virtual ~Actor() override;

	virtual void Init(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* texture) override;
	virtual void Update(const ScaldTimer& st) override;

	void SetTexture(ID3D11ShaderResourceView* texture);
	virtual void Draw(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix) override;
	
private:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11ShaderResourceView* mTexture = nullptr;
};