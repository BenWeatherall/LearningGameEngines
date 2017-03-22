#include "StaticMeshLoader.h"

StaticMeshLoader::StaticMeshLoader(std::string dir)
{
	Coords = new std::vector<coordinates*>;
	VertexObjects = new std::vector<StaticMesh*>;

	LoadDirectory(dir);
}

StaticMeshLoader::~StaticMeshLoader()
{
	for (unsigned int i = 0; i < VertexObjects->size(); ++i) {
		glDeleteVertexArrays(1, &(VertexObjects->at(i)->VAO));
		glDeleteBuffers(1, &(VertexObjects->at(i)->VBO));
	}
}

void StaticMeshLoader::LoadDirectory(std::string dir)
{

	std::vector<std::string> files = DirectoryContents(dir);

	std::vector<coordinates*> * LoadedVertices = new std::vector<coordinates*>;
	std::vector<elements*> * LoadedElements = new std::vector<elements*>;

	std::ifstream fb; // FileBuffer

	coordinates* CoordinateBuffer;
	elements* ElementBuffer;

	for (std::string file : files) {
		if (!std::regex_match(file, std::regex(".*\\.(dobj|obj)"))) {
			continue;
		}

		std::cout << "Loading: " << (dir + file) << std::endl;

		fb.open((dir + file), std::ios::in);

		if (fb.is_open()) {
			std::string ObjectName = file; // Save Object Name

			// Initialise file buffers
			std::string LineBuf;
			std::stringstream ss;
			std::string Unit;

			CoordinateBuffer = new coordinates;
			CoordinateBuffer->VertContainer = new std::vector<GLfloat>;
			CoordinateBuffer->Name = ObjectName;

			ElementBuffer = new elements;
			ElementBuffer->ElemContainer = new std::vector<GLuint>;
			ElementBuffer->Name = ObjectName;

			while (std::getline(fb, LineBuf)) {
				// We want to easily break this up so turn the line into a string stream
				ss.str(LineBuf);

				switch (LineBuf[0])
				{
				case 'v':
					// While this is small bit of code; it is used twice (or more) maybe move it into helper function?
					// TODO: func to break ss into values and push into vector. IN (SS, *Vector, TYPE?), OUT Void
					while (std::getline(ss, Unit, ' ')) {
						if (!(isalpha(Unit[0]) || Unit.size() == 0)) { 
							CoordinateBuffer->VertContainer->push_back(GLfloat(std::stof(Unit)));
						}
					}
					break;
				case 'p':
					while (std::getline(ss, Unit, ' ')) {
						if (!isalpha((Unit[0]))) {
							ElementBuffer->ElemContainer->push_back(GLuint(std::stoi(Unit)));
						}
					}
					break;
				default:
					break;
				}
				// You always need to clear stringstream: gets rid of all data and resets the getline method
				ss.clear();
			}

			CoordinateBuffer->VertData = CoordinateBuffer->VertContainer->data();
			CoordinateBuffer->VertSize = CoordinateBuffer->VertContainer->size();

			ElementBuffer->ElemData = ElementBuffer->ElemContainer->data();
			ElementBuffer->ElemSize = ElementBuffer->ElemContainer->size();
			
			LoadedVertices->push_back(CoordinateBuffer);
			LoadedElements->push_back(ElementBuffer);
			CoordinateBuffer = nullptr;
			ElementBuffer = nullptr;
		}
		else {
			std::cout << "Error: " << file << " failed to open" << std::endl;
		}
		


		fb.close();

	}

	Coords = LoadedVertices;
	Elems = LoadedElements;

	// Build Collected Coords into Vertex Objects
	BuildVertexObjects();

}

void StaticMeshLoader::BuildVertexObjects()
{
	StaticMesh* currentShape;
	std::cout << "Number of Coordinates: " << Coords->size() << std::endl;
	for (unsigned int i = 0; i < Coords->size(); ++i) {
		currentShape = new StaticMesh;
		currentShape->name = Coords->at(i)->Name;

		// Store the number of vertices for the shape (so we can easily call in future)
		currentShape->elements = Elems->at(i)->ElemSize;

		glGenVertexArrays(1, &(currentShape->VAO));
		glGenBuffers(1, &(currentShape->VBO));
		glGenBuffers(1, &(currentShape->EBO));

		// Bind Vertex Array Object first
		glBindVertexArray(currentShape->VAO);

		// Bind Vertex Buffer Object
		glBindBuffer(GL_ARRAY_BUFFER, currentShape->VBO);
		glBufferData(GL_ARRAY_BUFFER, Coords->at(i)->VertSize * sizeof(GLfloat), Coords->at(i)->VertData, GL_STATIC_DRAW);

		// Bind Element Buffer Object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentShape->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Elems->at(i)->ElemSize * sizeof(GLint), Elems->at(i)->ElemData, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0); // Unbind VAO (it's always a good idea to unbind any buffer / array to prevent strange bugs)

		VertexObjects->push_back(currentShape);

		currentShape = nullptr;
	}
}

std::vector<StaticMesh*> * StaticMeshLoader::GetShapes()
{
	return VertexObjects;
}
