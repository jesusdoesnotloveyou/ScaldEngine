#pragma once

#include "Graphics/ScaldCoreTypes.h"
#include "Graphics/Mesh.h"

namespace Scald
{
    class Shapes
    {
    public:
        static MeshData<VertexPositionNormalUV, DWORD> CreateBox(float width, float height, float depth);

        static MeshData<VertexPositionNormalUV, DWORD> CreateSphere(float radius, UINT sliceCount, UINT stackCount);

        ///< summary>
        /// Creates an mxn grid in the xz-plane with m rows and n columns, centered
        /// at the origin with the specified width and depth.
        ///</summary>
        static MeshData<VertexPositionNormalUV, DWORD> CreateGrid(float width, float depth, UINT m, UINT n);

        static MeshData<VertexPositionNormalUV, DWORD> CreateGeosphere(float radius, UINT numSubdivisions);
    private:
        static void Subdivide(MeshData<VertexPositionNormalUV, DWORD>& meshData);
        static VertexPositionNormalUV MidPoint(const VertexPositionNormalUV& v0, const VertexPositionNormalUV& v1);
    };
}