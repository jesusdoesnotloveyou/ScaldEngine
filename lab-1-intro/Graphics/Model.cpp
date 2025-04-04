#include "Model.h"

bool Model::Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture)
{
    pDevice = device;
    pDeviceContext = deviceContext;
    pTexture = texture;
    ThrowIfFailed(mCB.Init(pDevice, pDeviceContext));

    if (!LoadModel(filePath)) return false;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
    pTexture = texture;
}

void Model::Draw()
{
    pDeviceContext->PSSetShaderResources(0u, 1u, &pTexture);
    pDeviceContext->VSSetConstantBuffers(0u, 1u, mCB.GetAddressOf());

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

ConstantBuffer<ConstBufferVS>& Model::GetConstantBuffer()
{
    return mCB;
}