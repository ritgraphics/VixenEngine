/*
    The MIT License(MIT)

    Copyright(c) 2015 Vixen Team, Matt Guerrette

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <vix_pathmanager.h>
#include <vix_tinyxml.h>
#include <vix_debugutil.h>

#include <vix_file.h>

namespace Vixen
{

    std::filesystem::path PathManager::_AssetPath = "";
    std::filesystem::path PathManager::_ScenePath = "";
    std::filesystem::path PathManager::_ModelPath = "";
    std::filesystem::path PathManager::_MaterialPath = "";
    std::filesystem::path PathManager::_ShaderPath = "";
    std::filesystem::path PathManager::_ScriptPath = "";
    std::filesystem::path PathManager::_PrefabPath = "";

    PathManager::~PathManager()
    {
    }

    void PathManager::Initialize()
    {
        using namespace tinyxml2;

        // Open Environment Config File
        try
        {
            File envFile("vixen.env");

            const auto bytes = envFile.ReadAllBytes();

            XMLDOC      document;
            XMLError    err = document.Parse((char*)bytes.data(), bytes.size());
            std::string errMsg;
            if (XMLErrCheck(err, errMsg))
            {
                DebugPrintF("Env file failed to load: %s\n", errMsg.c_str());
                return;
            }

            const XMLElement* envElement = document.FirstChildElement("env");
            const XMLElement* assetPathElement = envElement->FirstChildElement("asset-path");

            _AssetPath += assetPathElement->Attribute("value");
            _AssetPath = std::filesystem::path(_AssetPath);

            _ScenePath = std::filesystem::path(_AssetPath).append("Scenes");

            _ModelPath = std::filesystem::path(_AssetPath).append("Models");

            _ShaderPath = std::filesystem::path(_AssetPath).append("Shaders");
#ifdef VIX_SYS_WINDOWS
            _ShaderPath.append("DirectX");
#endif
            _MaterialPath = std::filesystem::path(_AssetPath).append("Materials");
            _ScriptPath = std::filesystem::path(_AssetPath).append("Scripts");
            _PrefabPath = std::filesystem::path(_AssetPath).append("Prefabs");
        }
        catch (const std::exception& e)
        {
            SDL_Log("Failed to load environment file: %s", e.what());
        }
    }

    void PathManager::DeInitialize()
    {
    }

    std::filesystem::path PathManager::AssetPath()
    {
        return _AssetPath;
    }

    std::filesystem::path PathManager::ScenePath()
    {
        return _ScenePath;
    }

    std::filesystem::path PathManager::ModelPath()
    {
        return _ModelPath;
    }

    std::filesystem::path PathManager::ShaderPath()
    {
        return _ShaderPath;
    }

    std::filesystem::path PathManager::ScriptPath()
    {
        return _ScriptPath;
    }

    std::filesystem::path PathManager::MaterialPath()
    {
        return _MaterialPath;
    }

    std::filesystem::path PathManager::PrefabPath()
    {
        return _PrefabPath;
    }
} // namespace Vixen
