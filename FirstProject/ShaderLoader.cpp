#include "ShaderLoader.h"

// I will be using the algorithm shown at: http://insanecoding.blogspot.com.au/2011/11/how-to-read-in-file-in-c.html
shaders * load_shaders()
{
	std::string dir = "./Shaders/";
	shaders* LoadedShader = new shaders;

	std::vector<std::string> files = DirectoryContents(dir);
	
	std::ifstream fb; // FileBuffer

	// TODO: This will be repeated code; Need to fix
	std::cout << "File Count: " << files.size() << std::endl;
	for (std::string file : files) {
		if (!std::regex_match(file, std::regex(".*\\.(frag|vert)"))) {
			continue;
		}

		std::cout << "Loading: " << (dir + file) << std::endl;

		std::ifstream in((dir + file), std::ios::in | std::ios::binary);

		if (in) {
			in.seekg(0, std::ios::end);

			// TODO: Confirm I will never be dealing with a object file greater than MAX_INT bytes
			int length = in.tellg(); // tellg can return up to a long long. It is meant to be able to return the MAXIMUM POSSIBLE filesize the OS can handle. 
			GLchar * ShaderSourceCode = new GLchar [length+1]; // We are reading a c_string so make room for the \0

			in.seekg(0, std::ios::beg);

			in.read(ShaderSourceCode, length);

			if (!in) {
				std::cout << "error: only " << in.gcount() << " could be read";
			}

			in.close();
			ShaderSourceCode[length] = '\0'; // .read() doesn't add a \0, we need to add it ourselves


			// Build Shaders
			if (std::regex_match(file, std::regex(".*\\.vert"))) {
				BuildVertexShader(&(LoadedShader->vertexShader), &ShaderSourceCode);
			}
			if (std::regex_match(file, std::regex(".*\\.frag"))) {
				BuildFragmentShader(&(LoadedShader->fragmentShader), &ShaderSourceCode);
			}
			delete ShaderSourceCode;
		}
		else {
			std::cout << "Error: " << file << " failed to open" << std::endl;
		}

		fb.close();

	}

	return LoadedShader;
}

void BuildVertexShader(GLuint * VertexShader, GLchar** SourceCode)
{
	*VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*VertexShader, 1, SourceCode, NULL);
	glCompileShader(*VertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(*VertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(*VertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

}

void BuildFragmentShader(GLuint * FragmentShader, GLchar** SourceCode)
{
	*FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*FragmentShader, 1, SourceCode, NULL);
	glCompileShader(*FragmentShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(*FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(*FragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

}

GLuint BuildShaderProgram()
{
	shaders* LoadedShaders = load_shaders(); // TODO (since we are only passing around small values, this can be on the stack)

	GLuint ShaderProgram;
	ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, LoadedShaders->vertexShader);
	glAttachShader(ShaderProgram, LoadedShaders->fragmentShader);
	glLinkProgram(ShaderProgram);

	GLint success;
	GLchar infoLog[512];

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(LoadedShaders->vertexShader);
	glDeleteShader(LoadedShaders->fragmentShader);

	delete LoadedShaders;

	return ShaderProgram;
}
