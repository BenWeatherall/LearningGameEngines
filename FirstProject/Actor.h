#pragma once
#include <string>
#include <cctype>
#include "StaticMeshLoader.h"

class Actor {
public:
	Actor(std::string ActorDetails);
	~Actor();
	// Look at allowing homogenous vectos for animation (direction vs position)
	uint64_t ApplyAnimation(uint64_t Component, glm::vec4 Motion);
	void RemoveAnimation(uint64_t Animation);
	void tick(GLfloat delta);

private:
	//ComplexMesh*


};