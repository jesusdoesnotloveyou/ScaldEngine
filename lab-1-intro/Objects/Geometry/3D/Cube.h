#pragma once

#include "../SceneGeometry.h"

class Cube : public SceneGeometry
{
public:
	Cube();
	//Cube(const tuple<vector<Vertex>, vector<DWORD>>& vi);
	Cube(const tuple<vector<VertexTex>, vector<DWORD>>& vi);

	virtual ~Cube() override;

	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* pTexture = nullptr, const std::string& filePath = "") override;
	virtual void Update(const ScaldTimer& st) override;
	//~ End of SceneGeometry interface

private:
	void SetIndices();
	void SetVertices();
};