#pragma once
#include <string>
#include <cctype>
#include "Scene.h"
#include "ComplexMesh.h"

class Actor : Scene {
public:
	Actor(std::string ActorDetails);
	~Actor();
	// Look at allowing homogenous vectos for animation (direction vs position)
	// uint64_t ApplyAnimation(uint64_t Component, glm::vec4 Motion);
	// void RemoveAnimation(uint64_t Animation);
	// void tick(GLfloat delta);

private:
	//ComplexMesh*
	std::map<std::string, ComplexMesh*> Components;
};