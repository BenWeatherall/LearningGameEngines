#include "StaticMesh.h"

StaticMesh::StaticMesh(std::string model_file, std::vector<std::string> texture_file, std::vector<std::string> shader_filenames, ShaderLoader* scene_shader_loader)
{
	this->shader_program = scene_shader_loader->build_program(shader_filenames);


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
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->elements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void StaticMesh::build_mesh(std::string model_file)
{
	std::ifstream fb; // FileBuffer

	std::cout << "Loading: " << model_file << std::endl;
	std::ifstream in(model_file, std::ios::in | std::ios::binary);

	if (in) {
		in.seekg(0, std::ios::end);

		// TODO: Confirm I will never be dealing with a object file greater than MAX_INT bytes
		uint64_t length = in.tellg(); // tellg can return up to a long long. It is meant to be able to return the MAXIMUM POSSIBLE filesize the OS can handle. 
		GLchar * ShaderSourceCode = new GLchar[length + 1]; // We are reading a c_string so make room for the \0

		in.seekg(0, std::ios::beg);

		in.read(ShaderSourceCode, length);

		in.close();
		ShaderSourceCode[length] = '\0'; // .read() doesn't add a \0, we need to add it ourselves

										 // Build Shaders
		if (std::regex_match(filename, VERT_EXT))
		{
			built_shaders->operator[](filename) = build_shader(&ShaderSourceCode, GL_VERTEX_SHADER);
		}
		else if (std::regex_match(filename, FRAG_EXT))
		{
			built_shaders->operator[](filename) = build_shader(&ShaderSourceCode, GL_FRAGMENT_SHADER);
		}
		else
		{
			printf("ERROR: %s is not .frag nor .vert");
		}

		delete ShaderSourceCode;
	}
	else {
		std::cout << "ERROR: only " << in.gcount() << " could be read of " << filename << " : SKIPPING" << std::endl;
		in.close();
	}

	fb.close();



	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

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
	int width, height;

	unsigned char* image = SOIL_load_image(texture_file.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	this->texture.push_back(load_texture);
}
