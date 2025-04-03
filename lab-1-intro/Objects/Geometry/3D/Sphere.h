#pragma once

#include "../SceneGeometry.h"

class Sphere : public SceneGeometry
{
public:
	Sphere();
	Sphere(const tuple<vector<VertexTex>, vector<DWORD>>& vi);
	virtual ~Sphere() override;

	//~ Begin of SceneGeometry interface
	virtual void Init(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* pTexture = nullptr, const std::string& filePath = "") override;
	virtual void Update(const ScaldTimer& st) override;
	//~ End of SceneGeometry interface

private:

};