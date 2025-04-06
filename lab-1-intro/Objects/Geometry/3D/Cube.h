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
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath = "", const std::wstring& texturePath = L"") override;
	virtual void Update(const ScaldTimer& st) override;
	//~ End of SceneGeometry interface

private:
	void SetIndices();
	void SetVertices();
};