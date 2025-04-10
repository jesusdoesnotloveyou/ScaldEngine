#include "Model.h"
#include <WICTextureLoader.h>

bool Model::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string& modelFilePath, const std::wstring& textureFilePath)
{
    pDevice = device;
    pDeviceContext = deviceContext;

    ThrowIfFailed(mCB_VS.Init(pDevice, pDeviceContext));
    ThrowIfFailed(mCB_PS.Init(pDevice, pDeviceContext));
    
    ThrowIfFailed(CreateWICTextureFromFile(pDevice, textureFilePath.data(), nullptr, mTexture.GetAddressOf()));
    if (!LoadModel(modelFilePath)) return false;

    return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
    mTexture = texture;
}

void Model::Draw()
{
                                                 // &mTexture will delete texture, since & clears memory
    pDeviceContext->PSSetShaderResources(0u, 1u, mTexture.GetAddressOf());
    pDeviceContext->VSSetConstantBuffers(0u, 1u, mCB_VS.GetAddressOf());
    pDeviceContext->PSSetConstantBuffers(0u, 1u, mCB_PS.GetAddressOf());

    for (auto& mesh : mMeshes)
    {
        mesh.Draw();
    }
}

bool Model::LoadModel(const std::string& filePath)
{
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
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
    std::vector<VertexTex> vertices;
    std::vector<DWORD> indices;

    // Get vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        VertexTex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

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

ConstantBuffer<ConstBufferVS>& Model::GetConstantBufferVS()
{
    return mCB_VS;
}

ConstantBuffer<ConstBufferPS>& Model::GetConstantBufferPS()
{
    return mCB_PS;
}