#include "SceneLoader.h"

SceneLoader::SceneLoader(std::string SceneFile, Scene* loading_scene)
{
	this->scene = loading_scene;
	// Initialise Helper Objects
	scene_static_loader = new StaticMeshLoader();
	scene_shader_loader = new ShaderLoader();

	
	std::ifstream fb; // FileBuffer

	std::cout << "Loading: " << (SceneFile) << std::endl;

	fb.open((SceneFile), std::ios::in);

	if (fb.is_open()) {
		std::string ObjectName = SceneFile; // Save Object Name
		std::string LineBuf;
		bool load_success = true;

		while (std::getline(fb, LineBuf)) {
			if (LineBuf == "Actors")
			{
				load_success = BuildActors(&fb, LineBuf);
			}
			if (LineBuf == "Animations")
			{
				load_success = BuildAnimations(&fb, LineBuf);
			}
			if (LineBuf == "Camera:")
			{
				load_success = BuildCamera(&fb, LineBuf);
			}
			if (LineBuf == "Lights:")
			{
				load_success = BuildLights(&fb, LineBuf);
			}
			if (LineBuf == "SceneName:")
			{
				load_success = BuildSceneName(&fb, LineBuf);
			}
			
			if (LineBuf == "Skybox:")
			{
				load_success = BuildSkybox(&fb, LineBuf);
			}
			if (LineBuf == "Statics")
			{
				load_success = BuildStatics(&fb, LineBuf);
			}
			if (!load_success)
			{
				std::cout << "Scene Failed to Load" << std::endl;
				exit(-1);
			}
		}
	}
	else
	{
		std::cout << "Scene Failed to load!" << std::endl;
	}
}

bool SceneLoader::BuildActors(std::ifstream* fb, std::string LineBuf)
{
	return true;
}

bool SceneLoader::BuildAnimations(std::ifstream* fb, std::string LineBuf)
{
	return true;
}

bool SceneLoader::BuildCamera(std::ifstream* fb, std::string LineBuf)
{
	return true;
}

bool SceneLoader::BuildLights(std::ifstream* fb, std::string LineBuf)
{
	return true;
}

bool SceneLoader::BuildSceneName(std::ifstream* fb, std::string LineBuf)
{
	std::getline(*fb, LineBuf);
	std::cout << LineBuf << ":" << this->scene << std::endl;
	this->scene->scene_name = LineBuf;
	return true;
}

bool SceneLoader::BuildSkybox(std::ifstream* fb, std::string LineBuf)
{
	return true;
}

bool SceneLoader::BuildStatics(std::ifstream* fb, std::string LineBuf)
{
	std::regex static_regex("\t\w*, \w*, ([\d\.f], ?){9}");

	std::string obj_file;
	std::vector<std::string> texture_files, shader_files;
	glm::vec3 scale, location, rotation;

	std::streampos last_line = fb->tellg();
	std::getline(*fb, LineBuf);
	
	while (std::regex_match(LineBuf, static_regex))
	{
		// Get obj details
		sscanf(LineBuf.c_str(), "\t%s, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", 
				&obj_file, 
				&scale.x, &scale.y, &scale.z, 
				&location.x, &location.y, &location.z,
				&rotation.x, &rotation.y, &rotation.z);

		std::getline(*fb, LineBuf);
		std::stringstream ss (LineBuf);
		for (std::string each; std::getline(ss, each, ','); texture_files.push_back(each));

		std::getline(*fb, LineBuf);
		ss.str(LineBuf);
		ss.clear(); // Remember to clear to reset EOF flag
		for (std::string each; std::getline(ss, each, ','); shader_files.push_back(each));

		StaticMesh * new_static = new StaticMesh(obj_file, texture_files, shader_files, scale, location, rotation, this->scene_shader_loader, this->scene_static_loader);

		std::streampos last_line = fb->tellg();
		std::getline(*fb, LineBuf);
	}
	
	fb->seekg(last_line);

	return true;
}
