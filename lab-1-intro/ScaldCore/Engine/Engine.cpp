#include "Engine.h"

Engine::Engine()
    :
    renderWindow(800, 800, "testWindow1")
{}

int Engine::Launch()
{
	renderWindow.GetGfx().Setup();
	while (true)
	{
		// process all messages pending, but to not block
		if (const auto eCode = RenderWindow::ProcessMessages())
		{

			//Update();
			// if return optional has a value, means we're quitting
			return *eCode;
		}
		// otherwise
		RenderFrame();
	}
}

void Engine::Update()
{

}

void Engine::RenderFrame()
{
	const float c = static_cast<float>(sin(timer.Peek()) / 2.0f + 0.5f);
	renderWindow.GetGfx().ClearBuffer(c);
	renderWindow.GetGfx().Draw(timer.Peek());
	renderWindow.GetGfx().EndFrame();
}