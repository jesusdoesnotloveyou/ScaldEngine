#pragma once

#include "IWindowManager.h"

namespace Scald
{
	class WinApiWindowManager final : public IWindowManager
	{
    public:
        WinApiWindowManager() = default;
        ~WinApiWindowManager() override; 

	private:
	};
}