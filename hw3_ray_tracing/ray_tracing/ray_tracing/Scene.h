#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Object.h"

using namespace std;

class Scene {
public:
	glm::vec3
		lightPos = glm::vec3(0.0f, 3.0f, 0.0f),
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f),
		diffuseColor = lightColor * glm::vec3(0.5f), // decrease the influence
		ambientColor = diffuseColor * glm::vec3(0.2f), // low influence
		specularStrength = glm::vec3(1.0f, 1.0f, 1.0f);

	vector<Object*> objects;
	void add_object(Object* const object);
};

