#pragma once

#include "Mesh.h"
#include "ScaldCoreTypes.h"
#include <filesystem>

using Path = std::filesystem::path;

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

namespace Scald
{
    class Model;
    struct Texture;

    using Models = std::vector<Model>;
    using Textures = std::vector<Texture>;

    class AssetManager final
    {
    public:
        AssetManager(ID3D11Device* device);
        ~AssetManager() noexcept = default;

        bool LoadModel(const Path& path);
    private:
        void ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshData<>>& meshData);
        MeshData<> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, ETextureType typeName);

    public:
        Mesh UploadMesh(const MeshData<>& cpuMeshData);
        const Model* GetLoadedModel(const Path& path) const;
    private:
        Models m_models;
        Textures m_textures;
        ID3D11Device* m_device = nullptr;
    };
}