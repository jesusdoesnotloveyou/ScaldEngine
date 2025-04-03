#pragma once

#include "../ScaldCore/Engine/ScaldWindows.h"
#include "ScaldCoreTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>

class Mesh
{
private:
	Mesh(ID3D11DeviceContext* deviceContext);
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexTex>& vertices, std::vector<DWORD> indeces);
	Mesh(const Mesh& mesh);

	void Draw();

	VertexBuffer<VertexTex>& GetVertexBuffer();
	IndexBuffer& GetIndexBuffer();

	ID3D11ShaderResourceView* GetTexture() const;

private:
	ID3D11DeviceContext* mDeviceContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

	VertexBuffer<VertexTex> mVB;
	IndexBuffer mIB;
};