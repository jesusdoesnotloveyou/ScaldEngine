#pragma once

#include "IWindowManager.h"

namespace Scald
{
	class GLFWWindowManager final : public IWindowManager
	{
    public:
		~GLFWWindowManager() override;
	};
}