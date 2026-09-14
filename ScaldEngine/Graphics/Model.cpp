#include "stdafx.h"
#include "Model.h"
#include <WICTextureLoader.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace Scald;

bool Model::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string& modelFilePath, const std::wstring& textureFilePath)
{
    pDevice = device;
    pDeviceContext = deviceContext;

    ThrowIfFailed(mCBPerObject.Init(pDevice, pDeviceContext));

    if (!textureFilePath.empty())
    {
        ThrowIfFailed(CreateWICTextureFromFile(pDevice, textureFilePath.data(), nullptr, mTexture.GetAddressOf()));
    }
    if (!LoadModel(modelFilePath)) return false;

    return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
    mTexture = texture;
}

void Model::Draw() const
{
    // &mTexture will delete texture, since & clears memory
    pDeviceContext->PSSetShaderResources(0u, 1u, mTexture.GetAddressOf());
    pDeviceContext->VSSetConstantBuffers(0u, 1u, mCBPerObject.GetAddressOf());

    for (const auto& mesh : mMeshes)
    {
        mesh.Draw();
    }
}

bool Model::LoadModel(const std::string& filePath)
{
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(filePath, 
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_FlipUVs |
        aiProcess_ConvertToLeftHanded);
    if (!pScene) return false;

    ProcessNode(pScene->mRootNode, pScene);
    return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.push_back(ProcessMesh(mesh, scene));
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // Data to fill
    std::vector<VertexPositionNormalUV> vertices;
    std::vector<DWORD> indices;

    // Get vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        VertexPositionNormalUV vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mVertices[i].x;
        vertex.normal.y = mesh->mVertices[i].y;
        vertex.normal.z = mesh->mVertices[i].z;

        if (mesh->HasTextureCoords(0))
        {
            vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
        }
        vertices.push_back(vertex);
    }

    // Get indices
    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    return Mesh(pDevice, pDeviceContext, vertices, indices);
}

ConstantBuffer<ConstantBufferPerObject>& Model::GetConstantBufferVS()
{
    return mCBPerObject;
}