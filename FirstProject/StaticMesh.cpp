#include "StaticMesh.h"

StaticMesh::StaticMesh(std::string model_file, std::string fragment_shader, std::string vertex_shader)
{
	this->shader_program = BuildShaderProgram(fragment_shader, vertex_shader);
}
