#pragma once
#include <cstdlib>
#include <string>
#include <iostream>
#include <glm\glm.hpp>

#include "ShaderLoader.h"
#include "StaticMeshLoader.h"

class StaticMesh {
public:
	StaticMesh(	std::string model_file, std::vector<std::string> texture_file, 
				std::vector<std::string> shader_filenames, 
				ShaderLoader* scene_shader_loader, 
				StaticMeshLoader* scene_static_loader,
				glm::vec3* rot_key,	glm::vec3* loc_key);
	~StaticMesh();
	void draw();

private:
	std::string name;
	GLuint* VAO; // Single Value
	GLuint* VBO; // Verts, Normals, UVs
	GLuint vertices; // Num of Verts
	std::vector<GLuint> texture; // Look at making this a vector for layered textures
	GLuint shader_program;
	glm::vec3* rotation;
	glm::vec3* location;

	void build_texture(std::string texture_file);
};