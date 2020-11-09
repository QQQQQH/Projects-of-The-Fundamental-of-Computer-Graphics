#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tuple>

#include "Face.h"
#include "Ray.h"
#include "Shader.h"

using namespace std;

class Object {
protected:
	const float INF = 100000000.0f, EPS = 1e-7;
public:
	glm::vec3
		ambient = glm::vec3(1.0f, 0.5f, 0.31f),
		diffuse = ambient,
		specular = ambient * glm::vec3(0.5f);
	float shininess = 32.0;
	float
		kShade = 1.0f,
		kReflect = 0.0f,
		kRefract = 0.0f,
		refractiveIndex = 1.5f;
	glm::mat4 model = glm::mat4(1.0f);

	void set_model(const glm::mat4& model0);

	virtual void prepare_for_ray_tracing() = 0;
	virtual tuple<float, const Object*, glm::vec3> get_intersection(const Ray& ray) = 0;

	virtual void Draw(Shader& shader) = 0;
};

class Cube : public Object {
	static const float vertices[216];
	void set_up();
public:
	unsigned int VAO, VBO;
	Face faces[12];

	Cube();
	void prepare_for_ray_tracing();
	tuple<float, const Object*, glm::vec3> get_intersection(const Ray& ray);

	void Draw(Shader& shader);
};
#endif