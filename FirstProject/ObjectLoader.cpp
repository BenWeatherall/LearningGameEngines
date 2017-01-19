#include "ObjectLoader.h"

LoadedVertexObjects::LoadedVertexObjects(std::string dir)
{
	Coords = new std::vector<coordinates*>;
	VertexObjects = new std::vector<VertexedShape*>;

	LoadDirectory(dir);
}

LoadedVertexObjects::~LoadedVertexObjects()
{
	for (unsigned int i = 0; i < VertexObjects->size(); ++i) {
		glDeleteVertexArrays(1, &(VertexObjects->at(i)->VAO));
		glDeleteBuffers(1, &(VertexObjects->at(i)->VBO));
	}
}

void LoadedVertexObjects::LoadDirectory(std::string dir)
{

	std::vector<std::string> files = DirectoryContents(dir);

	std::vector<coordinates*> * LoadedVertices = new std::vector<coordinates*>;

	std::ifstream fb; // FileBuffer
	coordinates* CoordinateBuffer; //ShapeBuffer

	for (std::string file : files) {

		std::cout << "Loading: " << (dir + file) << std::endl;

		fb.open((dir + file), std::ios::in);

		if (fb.is_open()) {
			std::string ObjectName;
			std::getline(fb, ObjectName);
			std::cout << ObjectName << std::endl;

			CoordinateBuffer = new coordinates;
			CoordinateBuffer->container = new std::vector<GLfloat>;

			// TODO: refactor into parser
			// This is lovely but won't work for complex data. I need to build a parser if I want to have complex files
			for (std::string each;
				std::getline(fb, each, ' ');
				CoordinateBuffer->container->push_back(GLfloat(std::stof(each)))
				);

			CoordinateBuffer->data = CoordinateBuffer->container->data();
			CoordinateBuffer->size = CoordinateBuffer->container->size();
			LoadedVertices->push_back(CoordinateBuffer);

			CoordinateBuffer = nullptr;
		}
		else {
			std::cout << "Error: " << file << " failed to open" << std::endl;
		}

		fb.close();

	}

	Coords = LoadedVertices;

	// Build Collected Coords into Vertex Objects
	BuildVertexObjects();

}

std::vector<std::string> LoadedVertexObjects::DirectoryContents(std::string dir)
{
	/* TODO: Dir addresses in Windows differs to linux. will need to change how dirs are handled for each system
	Consider starting each relevant section with a replace where windows replaces '/' with '\\', and 
	linux replaces '\\' with '/' */

	std::vector<std::string> FileList;
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
		std::string searchPath = dir + "*";
		std::cout << searchPath << std::endl;

		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);

		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					FileList.push_back(fd.cFileName);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		else {
			std::cout << "INVALID_HANDLE_VALUE: " << GetLastError() << std::endl;
			std::cout << "See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx" << std::endl;
		}
	#endif

	return FileList;
}

void LoadedVertexObjects::BuildVertexObjects()
{
	VertexedShape* currentShape;
	std::cout << "Number of Coordinates: " << Coords->size() << std::endl;
	for (unsigned int i = 0; i < Coords->size(); ++i) {
		currentShape = new VertexedShape;

		// Store the number of vertices for the shape (so we can easily call in future)
		currentShape->vertices = Coords->at(i)->size / 3;

		glGenVertexArrays(1, &(currentShape->VAO));
		glGenBuffers(1, &(currentShape->VBO));

		// Bind Vertex Array Object first, then bind vertex buffer and attribute pointer(s).
		glBindVertexArray(currentShape->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, currentShape->VBO);
		glBufferData(GL_ARRAY_BUFFER, Coords->at(i)->size * sizeof(GLfloat), Coords->at(i)->data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0); // Unbind VAO (it's always a good idea to unbind any buffer / array to prevent strange bugs)

		VertexObjects->push_back(currentShape);

		currentShape = nullptr;
	}
}

std::vector<VertexedShape*> * LoadedVertexObjects::GetShapes()
{
	return VertexObjects;
}
