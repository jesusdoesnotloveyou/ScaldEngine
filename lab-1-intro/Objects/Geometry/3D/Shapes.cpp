#include "Shapes.h"

using std::vector;
using std::tuple;

tuple<vector<Vertex>, vector<DWORD>> Shapes::GetBoxShape(float width, float height, float depth, const XMVECTORF32 color)
{
	float w = 0.5f * width;
	float h = 0.5f * height;
	float d = 0.5f * depth;

	// 24 so for normals, tangents
	vector<Vertex> boxVertices
	{
		Vertex(-w / 2, -h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(-w / 2, +h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, +h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, -h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		
		Vertex(+w / 2, +h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(-w / 2, +h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(-w / 2, -h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, -h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),

		Vertex(-w / 2, -h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(-w / 2, -h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(-w / 2, +h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(-w / 2, +h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		
		Vertex(+w / 2, +h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, -h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, -h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, +h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),

		Vertex(-w / 2, +h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, +h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, +h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(-w / 2, +h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		
		Vertex(-w / 2, -h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, -h / 2, -d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(+w / 2, -h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
		Vertex(-w / 2, -h / 2, +d / 2, 1.0f, color.f[0], color.f[1], color.f[2], color.f[3]),
	};

	vector<DWORD> boxIndices
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		14, 13, 12,
		15, 14, 12,

		19, 18, 17,
		19, 17, 16,

		20, 21, 22,
		20, 22, 23
	};

	return { boxVertices, boxIndices };
}

tuple<vector<VertexTex>, vector<DWORD>> Shapes::GetBoxShape(float width, float height, float depth, float u, float v)
{
	float w = 0.5f * width;
	float h = 0.5f * height;
	float d = 0.5f * depth;

	// 24 so for normals, tangents
	vector<VertexTex> boxVertices
	{
		VertexTex(-w / 2, -h / 2, -d / 2, 1.0f, u, v),
		VertexTex(-w / 2, +h / 2, -d / 2, 1.0f, u, v),
		VertexTex(+w / 2, +h / 2, -d / 2, 1.0f, u, v),
		VertexTex(+w / 2, -h / 2, -d / 2, 1.0f, u, v),

		VertexTex(+w / 2, +h / 2, +d / 2, 1.0f, u, v),
		VertexTex(-w / 2, +h / 2, +d / 2, 1.0f, u, v),
		VertexTex(-w / 2, -h / 2, +d / 2, 1.0f, u, v),
		VertexTex(+w / 2, -h / 2, +d / 2, 1.0f, u, v),

		VertexTex(-w / 2, -h / 2, -d / 2, 1.0f, u, v),
		VertexTex(-w / 2, -h / 2, +d / 2, 1.0f, u, v),
		VertexTex(-w / 2, +h / 2, +d / 2, 1.0f, u, v),
		VertexTex(-w / 2, +h / 2, -d / 2, 1.0f, u, v),

		VertexTex(+w / 2, +h / 2, -d / 2, 1.0f, u, v),
		VertexTex(+w / 2, -h / 2, -d / 2, 1.0f, u, v),
		VertexTex(+w / 2, -h / 2, +d / 2, 1.0f, u, v),
		VertexTex(+w / 2, +h / 2, +d / 2, 1.0f, u, v),

		VertexTex(-w / 2, +h / 2, -d / 2, 1.0f, u, v),
		VertexTex(+w / 2, +h / 2, -d / 2, 1.0f, u, v),
		VertexTex(+w / 2, +h / 2, +d / 2, 1.0f, u, v),
		VertexTex(-w / 2, +h / 2, +d / 2, 1.0f, u, v),

		VertexTex(-w / 2, -h / 2, -d / 2, 1.0f, u, v),
		VertexTex(+w / 2, -h / 2, -d / 2, 1.0f, u, v),
		VertexTex(+w / 2, -h / 2, +d / 2, 1.0f, u, v),
		VertexTex(-w / 2, -h / 2, +d / 2, 1.0f, u, v),
	};

	vector<DWORD> boxIndices
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		14, 13, 12,
		15, 14, 12,

		19, 18, 17,
		19, 17, 16,

		20, 21, 22,
		20, 22, 23
	};

	return { boxVertices, boxIndices };
}

tuple<vector<Vertex>, vector<DWORD>> Shapes::GetSphereShape(float radius, int stackCount, int sliceCount, const XMVECTORF32 color)
{
	vector<Vertex> vertices{};
	vector<DWORD> indices{};

	const XMFLOAT4 shapeColor = XMFLOAT4(color);

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

			Vertex vertex;
			vertex.position.x = radius * sinTheta * cosPhi;
			vertex.position.y = radius * cosTheta;
			vertex.position.z = radius * sinTheta * sinPhi;
			vertex.color.x = shapeColor.x;
			vertex.color.y = shapeColor.y;
			vertex.color.z = shapeColor.z;

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

	return { vertices, indices };
}

tuple<vector<VertexTex>, vector<DWORD>> Shapes::GetSphereShape(float radius, int stackCount, int sliceCount, float u, float v)
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
			vertex.texCoord.x = u;
			vertex.texCoord.y = v;

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

	return { vertices, indices };
}