#pragma once

#include "../Objects/Geometry/SceneGeometry.h"

class Model : public SceneGeometry
{
public:
	Model();
	Model(const tuple<vector<VertexTex>, vector<DWORD>>& vi);
	virtual ~Model() override;

	virtual void Init(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* texture) override;
	virtual void Update(const ScaldTimer& st) override;

	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const XMMATRIX& viewProjectionMatrix);

private:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11ShaderResourceView* mTexture = nullptr;
};