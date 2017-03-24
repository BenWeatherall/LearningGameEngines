#pragma once
#include <string>
#include <glm\glm.hpp>
#include "ShaderLoader.h"

class StaticMesh {
public:
	StaticMesh(std::string model_file, std::vector<std::string> shader_filenames, ShaderLoader* scene_shader_loader);
	~StaticMesh();
	void draw();
private:
	std::string name;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint elements; // Look at making this optional
	GLuint texture; // Look at making this a vector for layered textures
	GLuint shader_program;
	glm::vec3 rotation;
	glm::vec3 location;
};