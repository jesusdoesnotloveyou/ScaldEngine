#include "Engine.h"

Engine::Engine()
    :
    renderWindow(800, 800, "testWindow1")
{}

int Engine::Launch()
{
	while (true)
	{
		// process all messages pending, but to not block
		if (const auto eCode = RenderWindow::ProcessMessages())
		{
			// if return optional has a value, means we're quitting
			return *eCode;
		}
		// otherwise
		PresentFrame();
	}
}

void Engine::PresentFrame()
{
	renderWindow.GetGfx().DrawRectangle();
	renderWindow.GetGfx().EndFrame();
}