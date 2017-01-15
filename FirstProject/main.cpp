#if _WIN64 || _WIN32
	#include <Windows.h>
#elif linux
	#include <sys/types.h>
	#include <dirent.h>
#endif // _WIN32 || _WIN64

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GLEW\include\GL\glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Window Dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core \n"
	"layout(location = 0) in vec3 position;"
	"void main()"
	"{"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);"
	"}\n\0";

const GLchar* fragmentShaderSource = "#version 330 core \n"
	"out vec4 color;\n"
	"void main()"
	"{"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
	"}\n\0";

// STRUCTS
/* Basic Shape Data*/
struct coordinates {
	std::vector<GLfloat>* container;
	GLfloat *data;
	size_t size;
};
/* Buffer / Array data for shapes stored on GPU */
struct VertexedShapes {
	GLuint VAO;
	GLuint VBO;
	GLuint vertices;
};

// Function Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
std::vector<coordinates*>* load_shapes();

int main()
{
	// Init GLFW
	glfwInit();
	// Set GLFW req options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	/* We can create as many windows as we like via this method; remember that each window will need to be made 
	current context to have actions applied to it, so really you should have a pointer / function that can be
	reused to perform the same actions if you want multiple windows.*/ 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr); // Window 1
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// TODO: Look into 'attaching windows'. Probably best done via multiple viewports eh?

	// I.E This is where the following actions shall occur
	glfwMakeContextCurrent(window);

	// Set required callbacks: Guessing this becomes a large switch which will then be converted into a hash (after all you always want rebindable keys!)
	// Following from that; I wonder how one does context... State machine?
	glfwSetKeyCallback(window, key_callback);

	// Initialise GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}

	// Set up viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/** DEFINE SHADERS **/
	// Vertex Shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link Shaders
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/** Content **/
	GLclampf RedBit = 0.0f;

	// Load Shapes
	std::vector<coordinates*>* ShapeData = load_shapes();

	// Get number of shapes
	const GLint ShapeCount = ShapeData->size();

	// Define buffer object
	std::vector<VertexedShapes*> StoredShapes;

	VertexedShapes* currentShape;
	for (int i = 0; i < ShapeCount; ++i) {
		currentShape = new VertexedShapes;
		
		// Store the number of vertices for the shape (so we can easily call in future)
		currentShape->vertices = ShapeData->at(i)->size / 3;

		glGenVertexArrays(1, &(currentShape->VAO));
		glGenBuffers(1, &(currentShape->VBO));

		// Bind Vertex Array Object first, then bind vertex buffer and attribute pointer(s).
		glBindVertexArray(currentShape->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, currentShape->VBO);
		glBufferData(GL_ARRAY_BUFFER, ShapeData->at(i)->size * sizeof(GLfloat), ShapeData->at(i)->data, GL_STATIC_DRAW); // TODO: Problem possibly here, is shape loading correctly?

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0); // Unbind VAO (it's always a good idea to unbind any buffer / array to prevent strange bugs)

		StoredShapes.push_back(currentShape);

		currentShape = nullptr;
	}

	// Program Loop
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();

		// Rendering Commands here
		glClearColor(RedBit, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		if (RedBit < 1.0f) {
			RedBit += 0.001f;
		}
		else {
			RedBit = 0.0f;
		}

		// Draw our first triangle
		glUseProgram(shaderProgram);

		for (int i = 0; i < ShapeCount; ++i) {
			glBindVertexArray(StoredShapes[i]->VAO);
			glDrawArrays(GL_TRIANGLES, 0, StoredShapes[i]->vertices);
			glBindVertexArray(0);
		}

		// Swap Buffers
		glfwSwapBuffers(window);
	}

	for (int i = 0; i < ShapeCount; ++i) {
		glDeleteVertexArrays(1, &(StoredShapes[i]->VAO));
		glDeleteBuffers(1, &(StoredShapes[i]->VBO));
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When user presses the escape key, we set the windowShouldClose property to true,
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<coordinates*> * load_shapes()
{
	// TODO Get list of Shapes from Shapes Directory
	// ./Shapes
	const std::string dir = "Shapes\\";

	std::vector<std::string> files;


	#ifdef linux
		DIR *dp;
		struct dirent *dirp;
		if ((dp = opendir(dir.c_str())) == NULL) {
			std::cout << "Error (" << errno << ") opening " << dir << std::endl;
			return errno;
		}

		while ((dirp = readdir(dp)) != NULL) {
			files.pushback(string(dirp->d_name));
		}
		closedir(dp);
	#elif _WIN32 || _WIN64
		std::string searchPath = dir +"*";
		std::cout << searchPath << std::endl;

		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);

		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					files.push_back(fd.cFileName);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		else {
			std::cout << "INVALID_HANDLE_VALUE: " << GetLastError() << std::endl;
			std::cout << "See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx" << std::endl;
		}
	#endif

	std::vector<coordinates*> * loadedShapes = new std::vector<coordinates*>;

	std::ifstream fb; // FileBuffer
	coordinates* shapeBuf; //ShapeBuffer

	for (std::string file : files) {

		std::cout << "Loading: " << (dir + file) << std::endl;

		fb.open((dir + file), std::ios::in);

		if (fb.is_open()) {
			shapeBuf = new coordinates;
			shapeBuf->container = new std::vector<GLfloat>;
			for (std::string each; std::getline(fb, each, ' '); shapeBuf->container->push_back(GLfloat(std::stof(each))));

			shapeBuf->data = shapeBuf->container->data();
			shapeBuf->size = shapeBuf->container->size();
			loadedShapes->push_back(shapeBuf);

			shapeBuf = nullptr;
		}
		else {
			std::cout << "Error: " << file << " failed to open" << std::endl;
		}

		fb.close();

	}

	return loadedShapes;
}
