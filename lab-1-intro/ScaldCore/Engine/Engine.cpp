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
	auto boxShape = Shapes::GetBoxShape(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	auto sphereShape = Shapes::GetSphereShape(1.0f, 16, 16, 0.0f, 0.0f);

	PrimitiveGeometry* box = new Cube(boxShape);
	box->ObjectTransform.Scale = { 15.0f, 15.0f, 15.0f };
	box->ObjectTransform.rotAngle = 60.0f;

	PrimitiveGeometry* sphere = new Sphere(sphereShape);
	sphere->ObjectTransform.Scale = { 10.0f, 10.0f, 10.0f };
	sphere->ObjectTransform.rotAngle = 60.0f;
	sphere->ObjectTransform.orbitAngle = 80.0f;
	sphere->ObjectTransform.orbitRadius = 50.0f;
	sphere->ObjectTransform.ParentTransform = &box->ObjectTransform;

	mGameObjects.push_back(sphere);
	mGameObjects.push_back(box);
	
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
	XMFLOAT3 tmp;

	// sun
	if (mRenderWindow.kbd.IsKeyPressed('1'))
	{
		mRenderWindow.GetGfx().mCamera.SetupAttachment(&mGameObjects[0]->ObjectTransform);
		if (XMMatrixDecompose(&outScale, &outQuat, &outTrans, mGameObjects[0]->ObjectTransform.mWorldMatrix))
		{
			XMStoreFloat3(&tmp, outScale);
			tmp.y = 0.0f;
			outScale = XMLoadFloat3(&tmp);
			mRenderWindow.GetGfx().mCamera.SetLookAtPosition(outTrans);
			mRenderWindow.GetGfx().mCamera.SetPosition(outTrans - outScale);
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