#pragma once
#include "StaticMesh.h"
#include "Scene.h"

class ComplexMesh : Scene {
public:
	ComplexMesh();
	~ComplexMesh();
	void tick(GLfloat delta);
	void draw();

private:
	std::map<std::string, StaticMesh*>* Components;
	glm::vec3* location;
	glm::vec3* rotation;
	glm::vec3* scale;
};