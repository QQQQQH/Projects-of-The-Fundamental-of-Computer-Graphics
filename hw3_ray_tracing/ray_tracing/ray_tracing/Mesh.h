#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>


#include "Shader.h"
using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

struct Face {
	glm::vec3 points[3];
	glm::vec3 norm;
	bool in_face(const glm::vec3& p) const;
};

class Mesh {
public:
	// mesh Data
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture>      textures;
	vector<Face>		 faces;
	unsigned int VAO;
	glm::mat4 model;

	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	void applyModel();
	// render the mesh
	void Draw(Shader& shader);

private:
	// render data 
	unsigned int VBO, EBO;

	// initializes all the buffer objects/arrays
	void getFaces();
	void setupMesh();
};
#endif