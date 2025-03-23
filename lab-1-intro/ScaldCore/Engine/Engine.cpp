#include "Engine.h"
#include <sstream>

#include "../../Objects/Geometry/2D/Rectangle.h"
#include "../../Objects/Geometry/3D/Cube.h"
#include "../../Objects/Geometry/3D/Sphere.h"
#include "../../Objects/Geometry/3D/Shapes.h"

#include <random>
#include <ctime>

Engine::Engine()
    :
    mRenderWindow(1600, 900, "Direct3DApp")
{}

int Engine::Launch()
{
	mRenderWindow.GetGfx().Setup();
	SetupScene();
	
	mTimer.Reset();
	while (true)
	{
		mTimer.Tick();
		// process all messages pending, but to not block
		if (const auto eCode = RenderWindow::ProcessMessages())
		{
		 	// if return optional has a value, means we're quitting
			return *eCode;
		}
		// otherwise
		CalculateFrameStats();
		PollInput();
		UpdateScene(mTimer);
		RenderFrame(mTimer);
	}
}

void Engine::SetupScene()
{
	auto boxShape = Shapes::GetBoxShape();
	auto sunShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Gold);
	auto mercuryShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::SandyBrown);
	auto venusShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::DarkOrange);
	auto earthShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Aqua);
	auto moonShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Gray);
	auto marsShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::DarkRed);
	auto jupiterShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Brown);
	auto saturnShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::BurlyWood);
	auto uranusShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Blue);
	auto neptuneShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Indigo);
	auto plutoShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::AntiqueWhite);

	PrimitiveGeometry* sun = new Sphere(sunShape);
	sun->ObjectTransform.Scale = { 15.0f, 15.0f, 15.0f };
	sun->ObjectTransform.rotationSpeed = 20.0f;

	PrimitiveGeometry* mercury = new Sphere(mercuryShape);
	mercury->ObjectTransform.Scale = { 1.0f, 1.0f, 1.0f };
	mercury->ObjectTransform.rotationSpeed = 100.0f;
	mercury->ObjectTransform.orbitSpeed = 10.0f;
	mercury->ObjectTransform.orbitRadius = 50.0f;
	mercury->ObjectTransform.ParentTransform = &sun->ObjectTransform;

	PrimitiveGeometry* moon = new Sphere(moonShape);
	moon->ObjectTransform.Scale = { 0.5f, 0.5f, 0.5f };
	moon->ObjectTransform.rotationSpeed = 25.0f;
	moon->ObjectTransform.orbitSpeed = 150.0f;
	moon->ObjectTransform.orbitRadius = 5.0f;
	moon->ObjectTransform.ParentTransform = &mercury->ObjectTransform;
	
	PrimitiveGeometry* venus = new Sphere(venusShape);
	venus->ObjectTransform.Scale = { 0.95f, 0.95f, 0.95f };
	venus->ObjectTransform.rotationSpeed = 25.0f;
	venus->ObjectTransform.orbitSpeed = 20.0f;
	venus->ObjectTransform.orbitRadius = 84.0f;
	venus->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* earth = new Sphere(earthShape);
	earth->ObjectTransform.Scale = { 1.0f, 1.0f, 1.0f };
	earth->ObjectTransform.rotationSpeed = 25.0f;
	earth->ObjectTransform.orbitSpeed = 15.0f;
	earth->ObjectTransform.orbitRadius = 114.0f;
	earth->ObjectTransform.ParentTransform = &sun->ObjectTransform;

	/*
	PrimitiveGeometry* mars = new Sphere(marsShape);
	mars->ObjectTransform.Scale = { 0.5f, 0.5f, 0.5f };
	mars->ObjectTransform.Rotation = { 0.0f, 2.0f, 0.0f };
	mars->ObjectTransform.Translation = { 131.0f, 0.0f, 0.0f };
	mars->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* jupiter = new Sphere(jupiterShape);
	jupiter->ObjectTransform.Scale = { 5.0f, 5.0f, 5.f };
	jupiter->ObjectTransform.Rotation = { 0.0f, 0.7f, 0.0f };
	jupiter->ObjectTransform.Translation = { 181.0f, 0.0f, 0.0f };
	jupiter->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* saturn = new Sphere(saturnShape);
	saturn->ObjectTransform.Scale = { 4.5f, 4.5f, 4.5f };
	saturn->ObjectTransform.Rotation = { 0.0f, 0.8f, 0.0f };
	saturn->ObjectTransform.Translation = { 210.0f, 0.0f, 0.0f };
	saturn->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* uranus = new Sphere(uranusShape);
	uranus->ObjectTransform.Scale = { 3.0f, 3.0f, 3.0f };
	uranus->ObjectTransform.Rotation = { 0.0f, 0.85f, 0.0f };
	uranus->ObjectTransform.Translation = { 230.0f, 0.0f, 0.0f };
	uranus->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* neptune = new Sphere(neptuneShape);
	neptune->ObjectTransform.Scale = { 3.0f, 3.0f, 3.0f };
	neptune->ObjectTransform.Rotation = { 0.0f, 0.85f, 0.0f };
	neptune->ObjectTransform.Translation = { 250.0f, 0.0f, 0.0f };
	neptune->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* pluto = new Sphere(plutoShape);
	pluto->ObjectTransform.Scale = { 0.1f, 0.1f, 0.1f };
	pluto->ObjectTransform.Rotation = { 0.0f, 6.0f, 0.0f };
	pluto->ObjectTransform.Translation = { 268.0f, 0.0f, 0.0f };
	pluto->ObjectTransform.ParentTransform = &sun->ObjectTransform;*/
	
	mGameObjects.push_back(sun);
	mGameObjects.push_back(mercury);
	mGameObjects.push_back(moon);
	mGameObjects.push_back(earth);
	mGameObjects.push_back(venus);
	/*mGameObjects.push_back(mars);
	mGameObjects.push_back(jupiter);
	mGameObjects.push_back(saturn);
	mGameObjects.push_back(uranus);
	mGameObjects.push_back(neptune);
	mGameObjects.push_back(pluto);*/

	for (auto geometry : mGameObjects)
	{
		if (!geometry) continue;
		geometry->Initialize(mRenderWindow.GetGfx().GetDevice(), mRenderWindow.GetGfx().GetDeviceContext());
	}
}

void Engine::PollInput()
{
	while (!mRenderWindow.kbd.IsKeyEmpty()) {
		const auto keyEvent = mRenderWindow.kbd.ReadKey();
		unsigned char keyCode = keyEvent.GetCode();
	}

#pragma region CameraRotation
	std::ostringstream oss;
	const auto mouseEvent = mRenderWindow.mouse.Read();
	if (mRenderWindow.mouse.IsRightPressed()) {
		if (mouseEvent.GetType() == Mouse::Event::Type::RawMove) 
		{
			mRenderWindow.GetGfx().mCamera.AdjustRotation((float)mouseEvent.GetPosY() * 0.01f, (float)mouseEvent.GetPosX() * 0.01f, 0.0f);
			oss << "Mouse X: " << mouseEvent.GetPosX() <<
				" Mouse Y: " << mouseEvent.GetPosY() << "\n";
			OutputDebugString(oss.str().c_str());
		}
	}
#pragma endregion CameraRotation

#pragma region CameraMovement
	const float cameraSpeed = 140.f;
	if (mRenderWindow.kbd.IsKeyPressed('W'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(mRenderWindow.GetGfx().mCamera.GetForwardVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('S'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(mRenderWindow.GetGfx().mCamera.GetBackwardVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('D'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(mRenderWindow.GetGfx().mCamera.GetRightVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('A'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(mRenderWindow.GetGfx().mCamera.GetLeftVector() * cameraSpeed * mTimer.DeltaTime());
	}
	if (mRenderWindow.kbd.IsKeyPressed('E'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(0.0f, cameraSpeed * mTimer.DeltaTime(), 0.0f);
	}
	if (mRenderWindow.kbd.IsKeyPressed('Q'))
	{
		mRenderWindow.GetGfx().mCamera.AdjustPosition(0.0f, -cameraSpeed * mTimer.DeltaTime(), 0.0f);
	}
#pragma endregion CameraMovement
}

void Engine::UpdateScene(const ScaldTimer& st)
{
	for (auto gameObject : mGameObjects)
	{
		gameObject->Update(st);
	}
}

void Engine::RenderFrame(const ScaldTimer& st)
{
	//const float color = static_cast<float>(sin(mTimer.DeltaTime()) + 1.0f);
	mRenderWindow.GetGfx().ClearBuffer(0.1f);
	mRenderWindow.GetGfx().DrawScene(mGameObjects);
	mRenderWindow.GetGfx().EndFrame();
}

void Engine::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the
	// average time it takes to render one frame. These stats
	// are appeneded to the window caption bar.
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS: " << fps << " "
			<< "Frame Time: " << mspf << "(ms)\n";
		mRenderWindow.SetTitle(outs.str());
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

float Engine::AspectRatio()const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}