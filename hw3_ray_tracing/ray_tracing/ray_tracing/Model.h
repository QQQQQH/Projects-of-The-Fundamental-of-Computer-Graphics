#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "Mesh.h"
#include "Object.h"
#include "Shader.h"
using namespace std;

class Model : public Object {
public:
	vector<Texture> textures_loaded;
	vector<Mesh>    meshes;
	string directory;
	bool gammaCorrection;

	Model(string const& path, const Material& material0, bool gamma = false);

	void setup_gl();
	void prepare_for_ray_tracing();
	void Draw(Shader& shader) const;

private:
	void loadModel(string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif