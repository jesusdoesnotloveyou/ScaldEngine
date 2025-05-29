#include "Shapes.h"

//tuple<vector<VertexTex>, vector<DWORD>> Shapes::GetBoxShape(float width, float height, float depth, float u, float v)
//{
//	float w = 0.5f * width;
//	float h = 0.5f * height;
//	float d = 0.5f * depth;
//
//	// 24 so for normals, tangents
//	vector<VertexTex> boxVertices
//	{
//		VertexTex(-w / 2, -h / 2, -d / 2, 1.0f, 0.0f, 1.0f),
//		VertexTex(-w / 2, +h / 2, -d / 2, 1.0f, 0.0f, 0.0f),
//		VertexTex(+w / 2, +h / 2, -d / 2, 1.0f, 1.0f, 0.0f),
//		VertexTex(+w / 2, -h / 2, -d / 2, 1.0f, 1.0f, 1.0f),
//
//		VertexTex(+w / 2, +h / 2, +d / 2, 1.0f, 0.0f, 1.0f),
//		VertexTex(-w / 2, +h / 2, +d / 2, 1.0f, 0.0f, 0.0f),
//		VertexTex(-w / 2, -h / 2, +d / 2, 1.0f, 1.0f, 0.0f),
//		VertexTex(+w / 2, -h / 2, +d / 2, 1.0f, 1.0f, 1.0f),
//
//		VertexTex(-w / 2, -h / 2, -d / 2, 1.0f, 0.0f, 1.0f),
//		VertexTex(-w / 2, -h / 2, +d / 2, 1.0f, 0.0f, 0.0f),
//		VertexTex(-w / 2, +h / 2, +d / 2, 1.0f, 1.0f, 0.0f),
//		VertexTex(-w / 2, +h / 2, -d / 2, 1.0f, 1.0f, 1.0f),
//
//		VertexTex(+w / 2, +h / 2, -d / 2, 1.0f, 0.0f, 1.0f),
//		VertexTex(+w / 2, -h / 2, -d / 2, 1.0f, 0.0f, 0.0f),
//		VertexTex(+w / 2, -h / 2, +d / 2, 1.0f, 1.0f, 0.0f),
//		VertexTex(+w / 2, +h / 2, +d / 2, 1.0f, 1.0f, 1.0f),
//
//		VertexTex(-w / 2, +h / 2, -d / 2, 1.0f, 0.0f, 1.0f),
//		VertexTex(+w / 2, +h / 2, -d / 2, 1.0f, 0.0f, 0.0f),
//		VertexTex(+w / 2, +h / 2, +d / 2, 1.0f, 1.0f, 0.0f),
//		VertexTex(-w / 2, +h / 2, +d / 2, 1.0f, 1.0f, 1.0f),
//
//		VertexTex(-w / 2, -h / 2, -d / 2, 1.0f, 0.0f, 1.0f),
//		VertexTex(+w / 2, -h / 2, -d / 2, 1.0f, 0.0f, 0.0f),
//		VertexTex(+w / 2, -h / 2, +d / 2, 1.0f, 1.0f, 0.0f),
//		VertexTex(-w / 2, -h / 2, +d / 2, 1.0f, 1.0f, 1.0f),
//	};
//
//	vector<DWORD> boxIndices
//	{
//		0, 1, 2,
//		0, 2, 3,
//
//		4, 5, 6,
//		4, 6, 7,
//
//		8, 9, 10,
//		8, 10, 11,
//
//		14, 13, 12,
//		15, 14, 12,
//
//		19, 18, 17,
//		19, 17, 16,
//
//		20, 21, 22,
//		20, 22, 23
//	};
//
//	return { boxVertices, boxIndices };
//}

pair<vector<VertexTex>, vector<DWORD>> Shapes::GetSphereShape(float radius, int stackCount, int sliceCount)
{
	vector<VertexTex> vertices{};
	vector<DWORD> indices{};

	for (int i = 0; i <= stackCount; ++i)
	{
		float theta = static_cast<float>(i) / stackCount * XM_PI;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);

		for (int j = 0; j <= sliceCount; ++j)
		{
			float phi = static_cast<float>(j) / sliceCount * 2.0f * XM_PI;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);

			VertexTex vertex;
			vertex.position.x = radius * sinTheta * cosPhi;
			vertex.position.y = radius * cosTheta;
			vertex.position.z = radius * sinTheta * sinPhi;

			vertices.push_back(vertex);
		}
	}

	for (int i = 0; i < stackCount; ++i)
	{
		for (int j = 0; j < sliceCount; ++j)
		{
			int first = (i * (sliceCount + 1)) + j;
			int second = first + sliceCount + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	return std::make_pair(vertices, indices);
}

void Shapes::GetSphereShape(std::vector<VertexTex>& outVertices, std::vector<DWORD>& outIndeces, float radius, int stackCount, int sliceCount)
{
	for (int i = 0; i <= stackCount; ++i)
	{
		float theta = static_cast<float>(i) / stackCount * XM_PI;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);

		for (int j = 0; j <= sliceCount; ++j)
		{
			float phi = static_cast<float>(j) / sliceCount * 2.0f * XM_PI;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);

			VertexTex vertex;
			vertex.position.x = radius * sinTheta * cosPhi;
			vertex.position.y = radius * cosTheta;
			vertex.position.z = radius * sinTheta * sinPhi;

			outVertices.push_back(vertex);
		}
	}

	for (int i = 0; i < stackCount; ++i)
	{
		for (int j = 0; j < sliceCount; ++j)
		{
			int first = (i * (sliceCount + 1)) + j;
			int second = first + sliceCount + 1;

			outIndeces.push_back(first);
			outIndeces.push_back(second);
			outIndeces.push_back(first + 1);

			outIndeces.push_back(second);
			outIndeces.push_back(second + 1);
			outIndeces.push_back(first + 1);
		}
	}
}