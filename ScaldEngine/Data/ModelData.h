#pragma once

#include <string>

namespace Scald
{
    class ModelData
    {
    public:
        ModelData(const std::string modelFilePath = "", const wchar_t* szTextureFileName = L"")
            : modelPath(modelFilePath),
              texturePath(szTextureFileName)
        {
        }

    public:
        std::string modelPath;
        std::wstring texturePath;
    };
}