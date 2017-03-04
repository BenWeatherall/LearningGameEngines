#pragma once
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

struct Polygon {
	GLfloat* Vertices;
	GLuint* Elements;
	GLuint Points;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};

Polygon * GeneratePolygon(unsigned int NumPoints)
{
	const float OUTER_DIAMETER = 1.0;
	const float Z = 0.0;

	Polygon * PolygonMesh = new Polygon;
	PolygonMesh->Vertices = new GLfloat[(NumPoints + 1)* 3];
	PolygonMesh->Elements = new GLuint[(NumPoints*3)];
	PolygonMesh->Points = NumPoints;
	GLfloat Angle = 2.0 * glm::pi<float>() / (GLfloat)NumPoints;	

	// Initial Point
	PolygonMesh->Vertices[0] = 0.0f;	
	PolygonMesh->Vertices[1] = OUTER_DIAMETER;
	PolygonMesh->Vertices[2] = Z;

	// Array Structure is Outer Point, Inner Point, ending with Zero Point
	// X, Y, Z, 
	// x, y, z,
	// ...
	// 0, 0, z
	// This is for the EBO algorithm to easily go
	// X, 0, 1 -- 1, 2, 3 -- 3, 4, 5 -- etc
	// where X is second last point, odds are inner points and evens are outer points
	
	for(unsigned int point = 1; point < NumPoints; ++point)
	{	
		// Outer Circle
		PolygonMesh->Vertices[(point)*3] = PolygonMesh->Vertices[(point-1)*3] * cos(Angle) - PolygonMesh->Vertices[(point-1)*3+1] * sin(Angle);
		PolygonMesh->Vertices[(point)*3+1] = PolygonMesh->Vertices[(point-1)*3] * sin(Angle) + PolygonMesh->Vertices[(point-1)*3+1] * cos(Angle);
		PolygonMesh->Vertices[(point)*3+2] = Z;
	}
	
	// Final Point (Null Point)
	PolygonMesh->Vertices[(NumPoints + 1) * 3 -3] = 0.0f;
	PolygonMesh->Vertices[(NumPoints + 1) * 3 -2] = 0.0f;
	PolygonMesh->Vertices[(NumPoints + 1) * 3 -1] = Z;

		
	// Initial Elements (Connect final inner point to first triangles)
	// Outer Triangle
	for(unsigned int element = 0; element < NumPoints-1; ++element)
	{
		// Outer Triangle
		PolygonMesh->Elements[element*3] = element;
		PolygonMesh->Elements[element*3+1] = element+1;
		PolygonMesh->Elements[element*3+2] = NumPoints; // Final Element (central point)
		
		// Debug Report
		/*
		std::cout << (element*3) << ":" << element << "\t";
		std::cout << (element*3+1) << ":" << element+1 << "\t";
		std::cout << (element*3+2) << ":" << NumPoints << "\n";
		*/
	}

	// Final Triangle (Null, FinalVert, InitialVert)
	PolygonMesh->Elements[NumPoints*3 - 3] = NumPoints -1;
	PolygonMesh->Elements[NumPoints*3 - 2] = 0;
	PolygonMesh->Elements[NumPoints*3 - 1] = NumPoints;

	// Debug Report
	/*
	std::cout << (NumPoints*3 -3) << ":" << NumPoints - 1 << "\t";
	std::cout << (NumPoints*3 -2) << ":" << 0 << "\t";
	std::cout << (NumPoints*3 -1) << ":" << NumPoints << "\n";
	*/
	
	return PolygonMesh;
}
