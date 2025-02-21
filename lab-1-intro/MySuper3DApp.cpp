// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Engine.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	Engine engine;
	engine.Launch();

	return -1;
}