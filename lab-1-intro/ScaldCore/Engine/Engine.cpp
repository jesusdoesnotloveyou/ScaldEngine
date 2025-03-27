#include "Engine.h"
#include <sstream>

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
	
	/*PrimitiveGeometry* jupiter = new Sphere(jupiterShape);
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
	
	mGameObjects.push_back(mercury);
	mGameObjects.push_back(jupiter);
	mGameObjects.push_back(saturn);
	mGameObjects.push_back(uranus);
	mGameObjects.push_back(neptune);
	mGameObjects.push_back(hanimed);
	mGameObjects.push_back(io);
	mGameObjects.push_back(europe);
	mGameObjects.push_back(titan);*/

	mGameObjects.push_back(sun);
	
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

	// sun
	if (mRenderWindow.kbd.IsKeyPressed('1'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[0]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[0]->ObjectTransform.mWorldMatrix))
		{
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans);
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