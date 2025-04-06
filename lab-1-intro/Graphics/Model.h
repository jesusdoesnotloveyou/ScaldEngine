#pragma once

#include "../ScaldCore/Engine/ScaldWindows.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>

// Model for scene object
class Model
{
public:
	Model() = default;
	~Model() = default;

	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string& modelFilePath, const std::wstring& textureFilePath);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw();

	ConstantBuffer<ConstBufferVS>& GetConstantBuffer();

private:
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	ConstantBuffer<ConstBufferVS> mCB;
	
	std::vector<Mesh> mMeshes;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
};