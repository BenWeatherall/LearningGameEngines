#include "Scene.h"

void Scene::draw()
{
}

Scene::Scene(std::string SceneCSV)
{
	ShaderLoader shader_loader();
	StaticMeshLoader static_loader();

	// Load Skybox (inverse box, and texture)
	// TODO For StaticMesh: Process (pass StaticMeshLoader, ShaderLoader)
	// For ComplexMesh: Process
	// Define Lights
	// Define Camera

}
