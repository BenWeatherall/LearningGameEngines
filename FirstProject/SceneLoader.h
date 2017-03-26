#pragma once
#include <regex>
#include <string>
#include <iostream>

#include "Scene.h"
#include "StaticMesh.h"

class SceneLoader
{
public:
	SceneLoader(std::string scene_file, Scene* loading_scene);
private:
	Scene * scene;
	bool BuildActors(std::ifstream* fb, std::string LineBuf);
	bool BuildAnimations(std::ifstream* fb, std::string LineBuf);
	bool BuildCamera(std::ifstream* fb, std::string LineBuf);
	bool BuildLights(std::ifstream* fb, std::string LineBuf);
	bool BuildSceneName(std::ifstream* fb, std::string LineBuf);
	bool BuildSkybox(std::ifstream* fb, std::string LineBuf);
	bool BuildStatics(std::ifstream* fb, std::string LineBuf);
};