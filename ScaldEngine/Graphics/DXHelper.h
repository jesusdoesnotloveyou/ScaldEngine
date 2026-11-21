#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3d.h>
#include <dxgi1_6.h>

#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <SimpleMath.h>

#ifndef D3D11_ALIGNMENT_SIZE
    #define D3D11_ALIGNMENT_SIZE 16
#endif

#ifndef SHADER_SLOT
    #define SHADER_SLOT(x)  (x)
#endif

#ifndef NUM_RESOURSES
    #define NUM_RESOURCES(x)  (x)
#endif