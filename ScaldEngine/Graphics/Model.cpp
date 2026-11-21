#include "stdafx.h"
#include "Model.h"

using namespace Scald;

void Model::AddMesh(Mesh&& mesh)
{
    m_meshes.emplace_back(mesh);
}

const std::vector<Mesh>& Model::GetMeshes() const
{
    return m_meshes;
}