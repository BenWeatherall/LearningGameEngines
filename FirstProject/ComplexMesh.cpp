#include "ComplexMesh.h"

ComplexMesh::ComplexMesh()
{
}

ComplexMesh::~ComplexMesh()
{
}

void ComplexMesh::tick(GLfloat delta)
{
}

void ComplexMesh::draw()
{
	for (auto component : *Components)
	{
		component.second->draw();
	}
}
