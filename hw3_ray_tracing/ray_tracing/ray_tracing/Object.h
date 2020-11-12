#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tuple>
#include <vector>

#include "Face.h"
#include "Ray.h"
#include "Shader.h"

using namespace std;

class Object {
protected:
	static const float INF, EPS;
	int cnt = 0;
	glm::vec3 maxv = glm::vec3(-INF), minv = glm::vec3(INF);

	void find_minmax();
public:
	glm::vec3
		ambient = glm::vec3(1.0f, 0.5f, 0.31f),
		diffuse = ambient,
		specular = ambient * glm::vec3(0.5f);
	float shininess = 32.0;
	float
		kShade = 0.8f,
		kReflect = 0.2f,
		kRefract = 0.0f,
		refractiveIndex = 1.5f;
	glm::mat4 model = glm::mat4(1.0f);

	vector<Face> faces;

	void set_model(const glm::mat4& model0);
	bool intersect_AABB(const Ray& ray) const;
	tuple<float, glm::vec3> get_intersection(const Ray& ray);

	virtual void prepare_for_ray_tracing() = 0;
	virtual void Draw(Shader& shader) const = 0;
};

class Cube : public Object {
	static const float vertices[216];
	void set_up();
public:
	unsigned int VAO, VBO;

	Cube();
	void prepare_for_ray_tracing();
	void Draw(Shader& shader) const;
};

#endif