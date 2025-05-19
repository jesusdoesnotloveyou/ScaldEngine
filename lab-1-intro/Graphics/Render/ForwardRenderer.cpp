#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
	: Renderer(device, deviceContext, width, height)
{
}
