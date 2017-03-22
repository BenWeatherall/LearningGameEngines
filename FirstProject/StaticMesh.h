#pragma once

struct StaticMesh {

};

class StaticMesh {
public:
	StaticMesh();
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