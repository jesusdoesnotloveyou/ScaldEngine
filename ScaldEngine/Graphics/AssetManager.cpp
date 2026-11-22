#include "stdafx.h"
#include "AssetManager.h"
#include "Model.h"
#include "ScaldException.h"
#include "Texture.h"
#include "VertexTypes.h"

#include <WICTextureLoader.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace Scald;

namespace
{
    const wchar_t* kAssetModelsPath = L"Data/Models/";
    const wchar_t* kAssetTexturesPath = L"Data/Textures/";
}

AssetManager::AssetManager(ID3D11Device* device)
    : m_device(device)
{
    m_models.reserve(128u);
    m_textures.reserve(128u);
}

// TODO: async or multithreaded access
bool AssetManager::LoadModel(const Path& path)
{
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_ConvertToLeftHanded);
    if (!pScene) return false;

    std::vector<MeshData<>> cpuMeshesData;
    ProcessNode(pScene->mRootNode, pScene, cpuMeshesData);
    
    Model model;
    for (const auto& meshData : cpuMeshesData)
    {
        model.AddMesh(std::move(UploadMesh(meshData)));
    }

    m_models.emplace_back(std::move(model));
    return true;
}

void AssetManager::ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshData<>>& meshData)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshData.emplace_back(std::move(ProcessMesh(mesh, scene)));
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, meshData);
    }
}

MeshData<> AssetManager::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<VertexPositionNormalUV> vertices;
    std::vector<DWORD> indices;
    std::vector<Texture> textures;

    // Get vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        VertexPositionNormalUV vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->HasTextureCoords(0))
        {
            vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.texCoord = XMFLOAT2{0.0f, 0.0f};
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

    /*if (mesh->mMaterialIndex >= 0)
    {
        if (aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex])
        {   
            for (int i = 0; i < aiTextureType::aiTextureType_UNKNOWN; i++)
                LoadTexturesFromMaterial(material, static_cast<aiTextureType>(i), textures);
        }
    }*/

    return MeshData(vertices, indices, textures);
}

// TODO: async or multithreaded access
void AssetManager::LoadTexturesFromMaterial(aiMaterial* mat, aiTextureType type, Textures& meshTextures)
{
    //for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    //{
    //    aiString str;
    //    mat->GetTexture(type, i, &str);
    //    
    //    Texture texture;

    //    // just placeholder, didn't mean to
    //    const wchar_t* szFileName;
    //    
    //    ThrowIfFailed(CreateWICTextureFromFile(m_device, szFileName, nullptr, texture.Srv.GetAddressOf()));

    //    //texture.Id = TextureFromFile(str.C_Str(), directory);
    //    texture.Path = str.C_Str();
    //    meshTextures.push_back(texture);
    //}
}

// TODO: async or multithreaded access
bool AssetManager::LoadTexture(const Path& path)
{
    Texture texture;

    HRESULT hr = CreateWICTextureFromFile(m_device, path.c_str(), nullptr, texture.Srv.GetAddressOf());
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to load texture with AssetManager::LoadTexture(const Path& path).");
        return false;
    }
    texture.Path = path;
    texture.Id = m_textures.size();
    //texture.Type = ;

    m_textures.emplace_back(std::move(texture));

    return true;
}

Mesh AssetManager::UploadMesh(const MeshData<>& cpuMeshData)
{
    return Mesh(m_device, cpuMeshData.Vertices, cpuMeshData.Indices);
}

const Model* AssetManager::GetLoadedModel(const Path& path) const
{
    return &m_models[0];
}

const Texture* AssetManager::GetLoadedTexture(const Path& path) const
{
    // return an index to a texture in contigious memory based on the path to the texture
    return &m_textures[0];
}