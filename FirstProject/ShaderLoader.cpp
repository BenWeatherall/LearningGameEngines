#include "ShaderLoader.h"

// TODO Look at updating to allow for multiple of each shader
// Consider bitwise shader loading along with a map of shaders? 
// is this too much? 
shaders load_shaders(std::map< GLuint, std::vector<std::string> > filenames)
{
	shaders LoadedShaders;

	for (GLuint shader_type = GL_FRAGMENT_SHADER; shader_type < GL_VERTEX_SHADER + 1; shader_type++)
	{
		for (int shader = 0; shader < filenames[shader_type].size(); shader++)
		{
			std::ifstream fb; // FileBuffer

			std::cout << "Loading: " << (filenames[shader_type][shader]) << std::endl;
			std::ifstream in(filenames[shader_type][shader], std::ios::in | std::ios::binary);

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
				LoadedShaders.vertexShader.push_back(BuildShader(&ShaderSourceCode, shader_type));

				delete ShaderSourceCode;
			}
			else {
				std::cout << "ERROR: only " << in.gcount() << " could be read of " << filenames[shader_type][shader] << " : SKIPPING" << std::endl;
				in.close();
				continue;
			}

			fb.close();

		}
	}
	return LoadedShaders;
}

GLuint BuildShader(GLchar** SourceCode, GLuint shader_type)
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

}

// TODO: Issues with current system. We load every shader every time it is used across all the objects within the scene
// Consider building a map of shaders while "Loading" and delete them all at the end?
GLuint BuildShaderProgram(std::vector<std::string> vertex_filenames, std::vector<std::string> fragment_filenames)
{
	shaders LoadedShaders = load_shaders(vertex_filenames, fragment_filenames);

	GLuint ShaderProgram;
	ShaderProgram = glCreateProgram();

	// Attach Vertex Shaders
	for (int shader = 0; shader < LoadedShaders.vertexShader.size(); shader++)
	{
		glAttachShader(ShaderProgram, LoadedShaders.vertexShader[shader]);
	}

	// Attach Fragment Shaders
	for (int shader = 0; shader < LoadedShaders.fragmentShader.size(); shader++)
	{
		glAttachShader(ShaderProgram, LoadedShaders.fragmentShader[shader]);
	}


	glLinkProgram(ShaderProgram);

	GLint success;
	GLchar infoLog[512];

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}


	// Delete Vertex Shaders
	for (int shader = 0; shader < LoadedShaders.vertexShader.size(); shader++)
	{
		glDeleteShader(LoadedShaders.vertexShader[shader]);
	}

	// Delete Fragment Shaders
	for (int shader = 0; shader < LoadedShaders.fragmentShader.size(); shader++)
	{
		glDeleteShader(LoadedShaders.fragmentShader[shader]);
	}

	return ShaderProgram;
}
