#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Ray.h"

class Object {
public:
	Model* model = nullptr;
	glm::vec3
		ambient = glm::vec3(1.0f, 1.0f, 1.0f),
		diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
		specular = glm::vec3(0.5f, 0.5f, 0.5f);
	float shininess = 32.0;
	float
		kShade = 0.6f,
		kReflect = 0.2f,
		kRefract = 0.2f,
		refractiveIndex = 1.5f;
	glm::mat4 modelTrans;

	Object() {}
	Object(Model* const  model0, glm::mat4 modelTrans0 = glm::mat4(1.0f)) :model(model0), modelTrans(modelTrans0) {}
	void set_model_trans(glm::mat4 modelTrans0);
};
#endif