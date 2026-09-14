#pragma once

#include "Mesh.h"

#include <string>
#include <vector>

struct aiNode;
struct aiMesh;
struct aiScene;

namespace Scald
{   
    using namespace Microsoft::WRL;
    using namespace DirectX;


    class Model
    {
    public:
        Model() = default;
        ~Model() = default;
        
        bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::string& modelFilePath, const std::wstring& textureFilePath);
        void SetTexture(ID3D11ShaderResourceView* texture);
        void Draw() const;
        
        ConstantBuffer<ConstantBufferPerObject>& GetConstantBufferVS();
        
    private:
        bool LoadModel(const std::string& filePath);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        
        ConstantBuffer<ConstantBufferPerObject> mCBPerObject;
        
        std::vector<Mesh> mMeshes;
        ComPtr<ID3D11ShaderResourceView> mTexture;
        
        ID3D11Device* pDevice = nullptr;
        ID3D11DeviceContext* pDeviceContext = nullptr;
    };
}