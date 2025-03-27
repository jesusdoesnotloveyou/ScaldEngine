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
	auto boxShape = Shapes::GetBoxShape(1.0f, 1.0f, 1.0f, Colors::Indigo);
	auto sunShape = Shapes::GetSphereShape(20.0f, 16, 16, Colors::Gold);
	auto mercuryShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Orchid);
	auto venusShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::DarkOrange);
	auto earthShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Aqua);
	auto moonShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Gray);
	auto marsShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::DarkRed);
	auto phobosShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Gray);
	auto deimosShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::IndianRed);
	auto jupiterShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Chocolate);
	auto saturnShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::BurlyWood);
	auto titanShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Goldenrod);
	auto uranusShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Blue);
	auto neptuneShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Indigo);
	auto plutoShape = Shapes::GetSphereShape(1.0f, 16, 16, Colors::Silver);

	PrimitiveGeometry* sun = new Sphere(sunShape);
	sun->ObjectTransform.Scale = { 18.0f, 18.0f, 18.0f };
	sun->ObjectTransform.rotationSpeed = 60.0f;
	
	PrimitiveGeometry* mercury = new Sphere(mercuryShape);
	mercury->ObjectTransform.Scale = { 0.5f, 0.5f, 0.5f };
	mercury->ObjectTransform.rotationSpeed = 55.0f;
	mercury->ObjectTransform.orbitRadius = 50.0f;
	mercury->ObjectTransform.ParentTransform = &sun->ObjectTransform;

	PrimitiveGeometry* venus = new Sphere(venusShape);
	venus->ObjectTransform.Scale = { 0.95f, 0.95f, 0.95f };
	venus->ObjectTransform.rotationSpeed = 50.0f;
	venus->ObjectTransform.orbitRadius = 84.0f;
	venus->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* earth = new Sphere(earthShape);
	earth->ObjectTransform.Scale = { 1.0f, 1.0f, 1.0f };
	earth->ObjectTransform.rotationSpeed = 45.0f;
	earth->ObjectTransform.orbitRadius = 114.0f;
	earth->ObjectTransform.ParentTransform = &sun->ObjectTransform;

	PrimitiveGeometry* moon = new Sphere(moonShape);
	moon->ObjectTransform.Scale = { 0.3f, 0.3f, 0.3f };
	moon->ObjectTransform.rotationSpeed = 25.0f;
	moon->ObjectTransform.orbitRadius = 7.0f;
	moon->ObjectTransform.ParentTransform = &earth->ObjectTransform;

	PrimitiveGeometry* mars = new Sphere(marsShape);
	mars->ObjectTransform.Scale = { 0.6f, 0.6f, 0.6f };
	mars->ObjectTransform.rotationSpeed = 35.0f;
	mars->ObjectTransform.orbitRadius = 150.0f;
	mars->ObjectTransform.ParentTransform = &sun->ObjectTransform;

	PrimitiveGeometry* phobos = new Sphere(phobosShape);
	phobos->ObjectTransform.Scale = { 0.2f, 0.2f, 0.2f };
	phobos->ObjectTransform.rotationSpeed = 60.0f;
	phobos->ObjectTransform.orbitRadius = 5.0f;
	phobos->ObjectTransform.ParentTransform = &mars->ObjectTransform;

	PrimitiveGeometry* deimos = new Sphere(deimosShape);
	deimos->ObjectTransform.Scale = { 0.25f, 0.25f, 0.25f };
	deimos->ObjectTransform.rotationSpeed = 25.0f;
	deimos->ObjectTransform.orbitRadius = 10.0f;
	deimos->ObjectTransform.ParentTransform = &mars->ObjectTransform;
	
	PrimitiveGeometry* jupiter = new Sphere(jupiterShape);
	jupiter->ObjectTransform.Scale = { 5.0f, 5.0f, 5.f };
	jupiter->ObjectTransform.rotationSpeed = 25.0f;
	jupiter->ObjectTransform.orbitRadius = 220.0f;
	jupiter->ObjectTransform.ParentTransform = &sun->ObjectTransform;

	PrimitiveGeometry* hanimed = new Sphere(moonShape);
	hanimed->ObjectTransform.Scale = { 0.2f, 0.2f, 0.2f };
	hanimed->ObjectTransform.rotationSpeed = 100.0f;
	hanimed->ObjectTransform.orbitRadius = 2.0f;
	hanimed->ObjectTransform.ParentTransform = &jupiter->ObjectTransform;

	PrimitiveGeometry* io = new Sphere(venusShape);
	io->ObjectTransform.Scale = { 0.1f, 0.1f, 0.1f };
	io->ObjectTransform.rotationSpeed = 50.0f;
	io->ObjectTransform.orbitRadius = 4.0f;
	io->ObjectTransform.ParentTransform = &jupiter->ObjectTransform;

	PrimitiveGeometry* europe = new Sphere(moonShape);
	europe->ObjectTransform.Scale = { 0.15f, 0.15f, 0.15f };
	europe->ObjectTransform.rotationSpeed = 25.0f;
	europe->ObjectTransform.orbitRadius = 5.0f;
	europe->ObjectTransform.ParentTransform = &jupiter->ObjectTransform;
	
	PrimitiveGeometry* saturn = new Sphere(saturnShape);
	saturn->ObjectTransform.Scale = { 4.2f, 4.2f, 4.2f };
	saturn->ObjectTransform.rotationSpeed = 20.0f;
	saturn->ObjectTransform.orbitRadius = 300.0f;
	saturn->ObjectTransform.ParentTransform = &sun->ObjectTransform;

	PrimitiveGeometry* titan = new Sphere(titanShape);
	titan->ObjectTransform.Scale = { 0.2f, 0.2f, 0.2f };
	titan->ObjectTransform.rotationSpeed = 100.0f;
	titan->ObjectTransform.orbitRadius = 5.0f;
	titan->ObjectTransform.ParentTransform = &saturn->ObjectTransform;

	PrimitiveGeometry* uranus = new Sphere(uranusShape);
	uranus->ObjectTransform.Scale = { 2.8f, 2.8f, 2.8f };
	uranus->ObjectTransform.rotationSpeed = 15.0f;
	uranus->ObjectTransform.orbitRadius = 380.f;
	uranus->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* neptune = new Cube(boxShape);
	neptune->ObjectTransform.Scale = { 2.5f, 2.5f, 2.5f };
	neptune->ObjectTransform.rotationSpeed = 8.0f;
	neptune->ObjectTransform.orbitRadius = 440.0f;
	neptune->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	PrimitiveGeometry* pluto = new Cube(plutoShape);
	pluto->ObjectTransform.Scale = { 0.1f, 0.1f, 0.1f };
	pluto->ObjectTransform.rotationSpeed = 30.0f;
	pluto->ObjectTransform.orbitRadius = 490.0f;
	pluto->ObjectTransform.ParentTransform = &sun->ObjectTransform;
	
	mGameObjects.push_back(sun);
	mGameObjects.push_back(mercury);
	mGameObjects.push_back(venus);
	mGameObjects.push_back(earth);
	mGameObjects.push_back(mars);
	mGameObjects.push_back(jupiter);
	mGameObjects.push_back(saturn);
	mGameObjects.push_back(uranus);
	mGameObjects.push_back(neptune);
	mGameObjects.push_back(pluto);
	mGameObjects.push_back(moon);
	mGameObjects.push_back(phobos);
	mGameObjects.push_back(deimos);
	mGameObjects.push_back(hanimed);
	mGameObjects.push_back(io);
	mGameObjects.push_back(europe);
	mGameObjects.push_back(titan);

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

#pragma region CameraOrbitalMovement
	XMVECTOR outScale;
	XMVECTOR outQuat;
	XMVECTOR outTrans;
	XMVECTOR offset;

	// sun
	if (mRenderWindow.kbd.IsKeyPressed('1'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[0]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[0]->ObjectTransform.mWorldMatrix))
		{
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			offset = XMLoadFloat3(&mGameObjects[0]->ObjectTransform.Scale);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans - offset);
		}
	}
	// mercury
	if (mRenderWindow.kbd.IsKeyPressed('2'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[1]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[1]->ObjectTransform.mWorldMatrix))
		{
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			offset = XMLoadFloat3(&mGameObjects[1]->ObjectTransform.Scale);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans - offset);
		}
	}
	// venus
	if (mRenderWindow.kbd.IsKeyPressed('3'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[2]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[2]->ObjectTransform.mWorldMatrix))
		{
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			offset = XMLoadFloat3(&mGameObjects[2]->ObjectTransform.Scale);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans - offset);
		}
	}
	// earth
	if (mRenderWindow.kbd.IsKeyPressed('4'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[3]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[3]->ObjectTransform.mWorldMatrix))
		{
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			offset = XMLoadFloat3(&mGameObjects[3]->ObjectTransform.Scale);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans - offset);
		}
	}
	// mars
	if (mRenderWindow.kbd.IsKeyPressed('5'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[4]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[4]->ObjectTransform.mWorldMatrix))
		{
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			offset = XMLoadFloat3(&mGameObjects[4]->ObjectTransform.Scale);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans - offset);
		}
	}
	// jupiter
	if (mRenderWindow.kbd.IsKeyPressed('6'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[5]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[5]->ObjectTransform.mWorldMatrix))
		{
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			offset = XMLoadFloat3(&mGameObjects[5]->ObjectTransform.Scale);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans - offset);
		}
	}
	// saturn
	if (mRenderWindow.kbd.IsKeyPressed('7'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[6]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[6]->ObjectTransform.mWorldMatrix))
		{
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			offset = XMLoadFloat3(&mGameObjects[6]->ObjectTransform.Scale);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans - offset);
		}
	}
	// free fps
	if (mRenderWindow.kbd.IsKeyPressed(VK_SPACE))
	{
		mRenderWindow.GetGfx().mCamera.ClearAttachment();
	}

#pragma endregion

#pragma region CameraRotation
	std::ostringstream oss;
	const auto mouseEvent = mRenderWindow.mouse.Read();
	if (mRenderWindow.mouse.IsRightPressed()) {
		if (mouseEvent.GetType() == Mouse::Event::Type::RawMove) 
		{
			mRenderWindow.GetGfx().mCamera.AdjustRotation((float)mouseEvent.GetPosY() * 0.01f, (float)mouseEvent.GetPosX() * 0.01f, 0.0f);
#if 0
			oss << "Mouse X: " << mouseEvent.GetPosX() <<
				" Mouse Y: " << mouseEvent.GetPosY() << "\n";
			OutputDebugString(oss.str().c_str());
#endif
		}
	}
#pragma endregion CameraRotation

#pragma region CameraMovement
	const float cameraSpeed = 120.f;
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

#pragma region CameraPosDebug
	std::ostringstream oss;
	const auto CameraPos = mRenderWindow.GetGfx().mCamera.GetPositionFloat3();
	oss << "Camera's position: " << CameraPos.x << ", " << CameraPos.y << ", " << CameraPos.z << "\n";
	OutputDebugString(oss.str().c_str());
#pragma endregion CameraPosDebug
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