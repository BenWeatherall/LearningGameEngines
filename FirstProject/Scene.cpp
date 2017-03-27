#include "Scene.h"

Scene::Scene()
{
	statics = new std::map<std::string, ComplexMesh*>;
}

Scene::~Scene()
{
	for (auto mesh : *statics) {
		removeStatic(mesh.first);
	}
	delete statics;
}

bool Scene::attachStatic(std::string new_name, ComplexMesh * new_mesh)
{
	std::pair<std::string, ComplexMesh*> tmp_static = std::make_pair(new_name, new_mesh );
	this->statics->insert(tmp_static);
	return true;
}

void Scene::removeStatic(std::string static_name)
{
	delete (statics->at(static_name));
	statics->erase(static_name);
}

void Scene::draw()
{
	for (auto mesh : *statics)
		mesh.second->draw();
}

void Scene::tick(GLfloat delta)
{
	// define time delta
	// call tick for each Actor
}