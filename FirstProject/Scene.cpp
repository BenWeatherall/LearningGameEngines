#include "Scene.h"

Scene::Scene(std::string SceneFile)
{
	// Initialise Helper Objects
	ShaderLoader shader_loader();
	StaticMeshLoader static_loader();

	// Initialise Scene Containers
	statics = new std::vector<StaticMesh*>;

	// Load Scene file
	// SceneLoader scene_loader(SceneFile, this);

}

Scene::~Scene()
{
	for (auto mesh : *statics) {
		delete mesh;
	}
	delete statics;
}

bool Scene::attachStatic(std::string)
{
	// TODO
	return true;
}

void Scene::removeStatic(StaticMesh *)
{
	//TODO
}

void Scene::draw()
{
	for (auto mesh : *statics)
		mesh->draw();
}

void Scene::tick(GLfloat delta)
{
	// define time delta
	// call tick for each Actor
}