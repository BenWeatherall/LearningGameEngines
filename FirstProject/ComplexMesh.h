#pragma once
#include "StaticMesh.h"

class ComplexMesh {
public:
	ComplexMesh();
	~ComplexMesh();
	void tick(GLfloat delta);

private:
	std::vector<StaticMesh> Components;	
};