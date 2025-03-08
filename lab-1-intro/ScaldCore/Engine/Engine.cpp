#include "Engine.h"
#include <sstream>

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
	static int i = 0;
	while (!renderWindow.mouse.IsEmpty())
	{
		const auto e = renderWindow.mouse.Read();
		switch (e.GetType())
		{
		case Mouse::Event::Type::Move:
		{
			std::ostringstream oss;
			oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
			renderWindow.SetTitle(oss.str());
			break;
		}
		case Mouse::Event::Type::Leave:
		{
			renderWindow.SetTitle("Left window region!");
			break;
		}
		case Mouse::Event::Type::WheelUp:
			i++;
			{
				std::ostringstream oss;
				oss << "Wheel Up: " << i;
				renderWindow.SetTitle(oss.str());
			}
			break;
		case Mouse::Event::Type::WheelDown:
			i--;
			{
				std::ostringstream oss;
				oss << "Wheel Down: " << i;
				renderWindow.SetTitle(oss.str());
			}
			break;
		}
	}
	const float c = static_cast<float>(sin(timer.Peek()) / 2.0f + 0.5f);
	renderWindow.GetGfx().ClearBuffer(c);
	renderWindow.GetGfx().Draw(timer.Peek());
	renderWindow.GetGfx().EndFrame();
}