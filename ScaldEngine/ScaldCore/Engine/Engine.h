#pragma once

#include "RenderWindow.h"
#include "ScaldTimer.h"
#include "Graphics/DXHelper.h"

class KatamariPlayer;
class SceneGeometry;
class PointLight;
class DirectionalLight;
class ModelData;

class Engine
{
public:
	Engine();
	~Engine();
	int Launch();
	
	// Katamari specific
	std::shared_ptr<KatamariPlayer> m_player = nullptr;
	std::unordered_map<std::string, std::unique_ptr<ModelData>> m_models;

private:
	void SetupScene();
	void PollInput();
	void UpdateScene(const ScaldTimer& st);
	void RenderFrame(const ScaldTimer& st);

	void CalculateFrameStats();
	float AspectRatio() const;

protected:
	RenderWindow mRenderWindow;
	ScaldTimer mTimer;

	int mClientWidth = 1600;
	int mClientHeight = 900;
};