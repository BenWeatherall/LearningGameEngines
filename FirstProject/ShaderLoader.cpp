#include "ShaderLoader.h"

ShaderLoader::ShaderLoader()
{
	built_shaders = new std::map<std::string, GLuint>;
}

// Deletes shaders and cleans up map
ShaderLoader::~ShaderLoader()
{
	// Delete Shaders
	for (std::map<std::string, GLuint>::iterator it = built_shaders->begin(); it != built_shaders->end(); ++it)
	{
		glDeleteShader(it->second);
	}

	delete built_shaders;
}

void ShaderLoader::add_shaders(std::vector<std::string> filenames)
{
	for (auto const& filename : filenames)
	{
		if (!is_shader_built)
			load_shader(filename);
	}
}

// Builds shader program after loading any unloaded shaders
GLuint ShaderLoader::build_program(std::vector<std::string> filenames)
{
	add_shaders(filenames);
	return build_shader_program(filenames);
}

// Loads a shader from full-path, determines shader type and calls build shader
// on loaded data. Adds the resulting shader to our map
void ShaderLoader::load_shader(std::string filename)
{
	std::ifstream fb; // FileBuffer

	std::cout << "Loading: " << filename << std::endl;
	std::ifstream in(filename, std::ios::in | std::ios::binary);

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
}

// Builds a shader of the specified type from the provided source
GLuint ShaderLoader::build_shader(GLchar** SourceCode, GLuint shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, SourceCode, NULL);
	glCompileShader(shader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::COMPILATION_FAILED (0x%04X)\n %s", shader_type, infoLog);
		glDeleteShader(shader);
	}

	return shader;
}

GLuint ShaderLoader::build_shader_program(std::vector<std::string> filenames)
{
	GLuint ShaderProgram;
	ShaderProgram = glCreateProgram();

	// Attach Fragment Shaders
	for (auto const& shader : filenames)
	{
		glAttachShader(ShaderProgram, built_shaders->at(shader));
	}

	glLinkProgram(ShaderProgram);

	// Detach Fragment Shaders (Required to be able to delete them in the long run)
	for (auto const& shader : filenames)
	{
		glDetachShader(ShaderProgram, built_shaders->at(shader));
	}

	GLint success;
	GLchar infoLog[512];

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	return ShaderProgram;
}

bool ShaderLoader::is_shader_built(std::string filename)
{
	std::map<std::string, GLuint>::iterator it;
	it = built_shaders->find(filename);
	if (it != built_shaders->end())
		return true;

	return false;
}
