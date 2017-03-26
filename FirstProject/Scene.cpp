#include "Scene.h"

Scene::Scene()
{
	statics = new std::vector<StaticMesh*>;
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