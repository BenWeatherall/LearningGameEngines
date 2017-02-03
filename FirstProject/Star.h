#pragma once
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Star {
	GLfloat* Vertices;
	GLfloat* Elements;
	GLuint Points;
}

Star* GenerateStar(int NumPoints)
{
	const float OUTER_DIAMETER = 1.0;
	const float INNER_DIAMETER = 0.5;
	const float Z = 0.0;

	Star StarMesh = new Star;
	StarMesh->Vertices = new GLfloat[(NumPoints*2 + 1)* 3];
	StarMesh->Elements = new GLfloat[(NumPoints*6)];
	StarMesh->Points = NumPoints;
	GLfloat Angle = 2.0*pi() / (GLfloat)NumPoints;	

	// Initial Point OUTER
	StarMesh->Vertices[0] = 0.0f;	
	StarMesh->Vertices[1] = OUTER_DIAMETER;
	StarMesh->Vertices[2] = Z;

	// Initial point INNER (Initial outer point shifted by a half angle)
	StarMesh->Vertices[3] = - INNER_DIAMETER * sin(Angle/2.0);
	StarMesh->Vertices[4] = INNER_DIAMETER * cos(Angle/2.0);
	StarMesh->Vertices[5] = Z;

	// Array Structure is Outer Point, Inner Point, ending with Zero Point
	// X, Y, Z, 
	// x, y, z,
	// ...
	// 0, 0, z
	// This is for the EBO algorithm to easily go
	// X, 0, 1 -- 1, 2, 3 -- 3, 4, 5 -- etc
	// where X is second last point, odds are inner points and evens are outer points
	/
	for(unsigned int point = 1; point < NumPoints; ++points)
	{	
		// Outer Circle
		StarMesh->Vertices[(point)*6] = sin(StarMesh->Vertices[(point-1)*6] * cos(Angle) - StarMesh->Vertices[(point-1)*6+1] * sin(Angle);
		StarMesh->Vertices[(point)*6+1] = cos(StarMesh->Vertices[(point-1)*6] * sin(Angle) + StarMesh->Vertices[(point-1)*6+1] * cos(Angle);
		StarMesh->Vertices[(point)*6+2] = Z;

		// Inner Circle
		StarMesh->Vertices[(point)*6+3] = sin(StarMesh->Vertices[(point-1)*6+3] * cos(Angle) - StarMesh->Vertices[(point-1)*6+4] * sin(Angle);
		StarMesh->Vertices[(point)*6+4] = cos(StarMesh->Vertices[(point-1)*6+3] * sin(Angle) + StarMesh->Vertices[(point-1)*6+4] * cos(Angle);
		StarMesh->Vertices[(point)*6+5] = Z;
	}
	
	// Final Point (Null Point)
	StarMesh->Vertices[(NumPoints*6)-3] = 0.0f;
	StarMesh->Vertices[(NumPoints*6)-2] = 0.0f;
	StarMesh->Vertices[(NumPoints*6)-1] = Z;

	
	// Initial Elements (Connect final inner point to first triangles)
	// Outer Triangle
	StarMesh->Elements[0] = StarMesh->Points-2;
	StarMesh->Elements[1] = 0;
	StarMesh->Elements[2] = 1;

	// Inner Triangle
	StarMesh->Elements[3] = StarMesh->Points-2;
	StarMesh->Elements[4] = StarMesh->Points-1;
	StarMesh->Elements[5] = 1;

	for(unsigned int element = 1; element < NumPoints; ++element)
	{
		// Outer Triangle
		StarMesh->Elements[element*6] = element+1;
		StarMesh->Elements[element*6+1] = element+2;
		StarMesh->Elements[element*6+2] = element+3;
		
		// Inner Triangle
		StarMesh->Elements[element*6+3] = element+1;
		StarMesh->Elements[element*6+4] = StarMesh->Points-1;
		StarMesh->Elements[element*6+5] = element+3;
	}
	
	return StarMesh;
}
