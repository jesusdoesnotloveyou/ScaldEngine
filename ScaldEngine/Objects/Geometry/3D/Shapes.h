#pragma once

#include <d3d11.h>
#include <tuple>
#include <vector>

#include "../../../Graphics/ScaldCoreTypes.h"

using std::vector;
using std::tuple;
using std::pair;

class Shapes
{
public:
	// vertexTex
	static std::tuple<std::vector<VertexTex>, std::vector<DWORD>>GetBoxShape(float width = 1.0f, float height = 1.0f, float depth = 1.0f, float u = 0.0f,  float v = 0.0f);
	static std::pair<std::vector<VertexTex>, std::vector<DWORD>>GetSphereShape(float radius = 1.0f, int stackCount = 16, int sliceCount = 16);
	static void GetSphereShape(std::vector<VertexTex>& outVertices, std::vector<DWORD>& outIndeces, float radius = 1.0f, int stackCount = 16, int sliceCount = 16);
};