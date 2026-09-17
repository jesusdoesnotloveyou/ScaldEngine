#pragma once

#include "Graphics/DXHelper.h"
#include "Graphics/ScaldCoreTypes.h"

#include <tuple>
#include <vector>

namespace Scald
{
    class Shapes
    {
    public:
        // vertexTex
        static std::tuple<std::vector<VertexPositionNormalUV>, std::vector<DWORD>> GetBoxShape(float width = 1.0f, float height = 1.0f, float depth = 1.0f, float u = 0.0f, float v = 0.0f);
        static std::pair<std::vector<VertexPositionNormalUV>, std::vector<DWORD>> GetSphereShape(float radius = 1.0f, int stackCount = 16, int sliceCount = 16);
        static void GetSphereShape(std::vector<VertexPositionNormalUV>& outVertices, std::vector<DWORD>& outIndeces, float radius = 1.0f, int stackCount = 16, int sliceCount = 16);
    };
}