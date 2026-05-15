#include "stdafx.h"
#include "ScaldCore/Engine/Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ThrowIfFailed(CoInitialize(nullptr));

    Engine engine;
    engine.Launch();

    return 0;
}