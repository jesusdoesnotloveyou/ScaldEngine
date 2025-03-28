#pragma once

#include <d3d11.h>
#include <tuple>
#include <vector>

#include "../../../Graphics/ScaldCoreTypes.h"

class Shapes
{
public:
	static std::tuple<std::vector<Vertex>, std::vector<DWORD>>GetBoxShape(float width = 1.0f, float height = 1.0f, float depth = 1.0f, const XMVECTORF32 color = Colors::White);
	static std::tuple<std::vector<Vertex>, std::vector<DWORD>>GetSphereShape(float radius = 1.0f, int stackCount = 16, int sliceCount = 16, const XMVECTORF32 color = Colors::White);
	
	// vertexTex
	static std::tuple<std::vector<VertexTex>, std::vector<DWORD>>GetBoxShape(float width = 1.0f, float height = 1.0f, float depth = 1.0f, float u = 0.0f,  float v = 0.0f);
	static std::tuple<std::vector<VertexTex>, std::vector<DWORD>>GetSphereShape(float radius = 1.0f, int stackCount = 16, int sliceCount = 16, float u = 0.0f, float v = 0.0f);
};