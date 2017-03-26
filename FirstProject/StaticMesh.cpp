#include "StaticMesh.h"

StaticMesh::StaticMesh(	std::string model_file, 
						std::vector<std::string> texture_files, 
						std::vector<std::string> shader_filenames, 
						ShaderLoader* scene_shader_loader, StaticMeshLoader* scene_static_loader,
						glm::vec3* rot_key, 
						glm::vec3* loc_key )
{
	this->rotation = rot_key;
	this->location = loc_key;

	this->shader_program = scene_shader_loader->build_program(shader_filenames);
	scene_static_loader->build_static_mesh(model_file, this->VAO, this->VBO);

	for(auto texture : texture_files)
		build_texture(texture);

}

StaticMesh::~StaticMesh()
{
	glDeleteProgram(this->shader_program);
}

void StaticMesh::draw()
{
	// Only 32 GL_TEXTURE# are defined (0x84C0 -> 0x84DF)
	for (unsigned int tex_num = 0; tex_num < texture.size() && tex_num < 32; tex_num++)
	{
		std::string texture_name = "texture_" + std::to_string(tex_num);
		glActiveTexture(GL_TEXTURE0+tex_num);
		glBindTexture(GL_TEXTURE_2D, texture.at(tex_num));
		glUniform1i(glGetUniformLocation(this->shader_program, texture_name.c_str()), 0);
	}
	
	glUseProgram(this->shader_program);
	glBindVertexArray(*this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices);
	glBindVertexArray(0);
}

// TODO Replace with Scene function
void StaticMesh::build_texture(std::string texture_file)
{
	GLuint load_texture;
	glGenTextures(1, &load_texture);
	glBindTexture(GL_TEXTURE_2D, load_texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load, create texture and generate mipmaps
	int width, height, bpp;

	unsigned char* image = stbi_load(texture_file.c_str(), &width, &height, &bpp, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	this->texture.push_back(load_texture);
}
