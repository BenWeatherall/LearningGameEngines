#include "StaticMeshLoader.h"

StaticMeshLoader::StaticMeshLoader()
{
	built_meshes = new std::map<std::string, VertexObjects>;
}

// TODO: We need to track attachment and detachment of shaders
StaticMeshLoader::~StaticMeshLoader()
{
	// Unlike Shaders we need to hold onto the VAO / VBO's past the end of this structures life (Will need to consider how best to handle)
	delete built_meshes;
}

void StaticMeshLoader::build_static_mesh(std::string filename, GLuint* VAO, GLuint* VBO)
{
	if (!is_static_mesh_built(filename))
		load_mesh(filename);

	// TODO: If we start tracking attachment, Do it here
	VAO = built_meshes->at(filename).VAO;
	VBO = built_meshes->at(filename).VBO;
}

bool StaticMeshLoader::is_static_mesh_built(std::string filename)
{
	std::map<std::string, VertexObjects>::iterator it;
	it = built_meshes->find(filename);
	if (it != built_meshes->end())
		return true;

	return false;
}


void StaticMeshLoader::load_mesh(std::string filename)
{

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	
	std::ifstream fb; // FileBuffer
	int matches;

	std::cout << "Loading: " << (filename) << std::endl;

	fb.open((filename), std::ios::in);

	if (fb.is_open()) {
		std::string ObjectName = filename; // Save Object Name

		// Initialise file buffer
		std::string LineBuf;

		// Initialise vertex, uv, normal buffers
		std::string vertex1, vertex2, vertex3;
		glm::vec3 vertex, normal;
		glm::vec2 uv;
		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

		while (std::getline(fb, LineBuf)) {
			switch (LineBuf[0])
			{
			case 'v':
				// While this is small bit of code; it is used twice (or more) maybe move it into helper function?
				// TODO: func to break ss into values and push into vector. IN (SS, *Vector, TYPE?), OUT Void
				if (LineBuf[1] == 't') // vt (uv data)
				{
					sscanf(LineBuf.c_str(), "vt %f %f\n", &uv.x, &uv.y);
					temp_uvs.push_back(uv);
				}
				else if (LineBuf[1] == 'n')
				{
					sscanf(LineBuf.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_vertices.push_back(normal);
				}
				else // v (vertex data)
				{
					sscanf(LineBuf.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				break;
			case 'f':
				matches = sscanf(LineBuf.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					break;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				break;
			default:
				break;
			}
		}
	}
	else {
		std::cout << "Error: " << filename << " failed to open" << std::endl;
	}
		
	fb.close();

	// Now we have all out verts and all out indices we need to start duplicating
	// Based on our indices for generation of our mesh

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		uvs.push_back(uv);
	}

	// Build Collected Coords into Vertex Objects
	build_vertex_buffer(filename, &vertices, &normals, &uvs);
}

void StaticMeshLoader::build_vertex_buffer(	std::string filename, 
											std::vector< glm::vec3 >* vertices, 
											std::vector< glm::vec3 >* normals, 
											std::vector< glm::vec2 >* uvs)
{
	VertexObjects temp_VO;
	temp_VO.VAO = new GLuint;
	temp_VO.VBO = new GLuint[3];

	// Generate and Bind our VAO
	glGenVertexArrays(1, temp_VO.VAO);
	glBindVertexArray(*temp_VO.VAO);

	// Generate our VBOs
	glGenBuffers(3, temp_VO.VBO);

	// Bind Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, temp_VO.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(glm::vec3), vertices->data(), GL_STATIC_DRAW);

	// Bind Normal Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, temp_VO.VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(glm::vec3), normals->data(), GL_STATIC_DRAW);

	// Bind UV Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, temp_VO.VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs->size() * sizeof(glm::vec2), uvs->data(), GL_STATIC_DRAW);


	// Clean up
	glEnableVertexAttribArray(0); // Turn off Vertex Attribute Arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind Buffer Object
	glBindVertexArray(0); // Unbind VAO

	built_meshes->operator[](filename) = temp_VO;
}