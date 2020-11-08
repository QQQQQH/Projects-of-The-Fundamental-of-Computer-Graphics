#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

class Object {
public:
	Model* model = nullptr;
	glm::vec3
		ambient = glm::vec3(1.0f, 1.0f, 1.0f),
		diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
		specular = glm::vec3(0.5f, 0.5f, 0.5f);
	float shininess = 32.0;
	Object() {}
	Object(Model* const  model0) :model(model0) {}
};
#endif