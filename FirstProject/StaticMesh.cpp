#include "StaticMesh.h"

StaticMesh::StaticMesh(std::string model_file, std::vector<std::string> shader_filenames, ShaderLoader* scene_shader_loader)
{
	this->shader_program = scene_shader_loader->build_program(shader_filenames);
}

StaticMesh::~StaticMesh()
{
	glDeleteProgram(this->shader_program);
}

void StaticMesh::draw()
{
	glUseProgram(this->shader_program);
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->elements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
