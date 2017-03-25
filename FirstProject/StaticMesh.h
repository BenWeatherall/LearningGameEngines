#pragma once
#include <string>
#include <glm\glm.hpp>
#include <SOIL\SOIL.h>

#include "ShaderLoader.h"

class StaticMesh {
public:
	StaticMesh(std::string model_file, std::vector<std::string> texture_file, std::vector<std::string> shader_filenames, ShaderLoader* scene_shader_loader);
	~StaticMesh();
	void draw();
	void set_rot(glm::vec3 rotation);
	void set_pos(glm::vec3 position);
	glm::vec3 get_rot();
	glm::vec3 get_pos();

private:
	std::string name;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint elements; // Look at making this optional
	std::vector<GLuint> texture; // Look at making this a vector for layered textures
	GLuint shader_program;
	glm::vec3 rotation;
	glm::vec3 location;

	void build_mesh(std::string model_file);
	void build_texture(std::string texture_file);
};