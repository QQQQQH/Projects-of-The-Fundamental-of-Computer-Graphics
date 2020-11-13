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
#include "Material.h"
#include "Ray.h"
#include "Shader.h"

using namespace std;

class Object {
protected:
	static const float INF, EPS;
	const Material material;
	glm::vec3 maxv = glm::vec3(-INF), minv = glm::vec3(INF);

	virtual void find_minmax();
public:
	glm::mat4 model = glm::mat4(1.0f);
	vector<Face> faces;

	Object() {}
	Object(const Material& material0) :material(material0) {}

	glm::vec3 ambient() const { return material.ambient; }
	glm::vec3 diffuse() const { return material.ambient; }
	glm::vec3 specular() const { return material.specular; }
	float shininess() const { return material.shininess; }
	float kShade() const { return material.kShade; }
	float kReflect() const { return material.kReflect; }
	float kRefract() const { return material.kRefract; }
	float refractiveIndex() const { return material.refractiveIndex; }

	void set_model(const glm::mat4& model0);
	bool intersect_AABB(const Ray& ray) const;

	virtual bool get_intersection(const Ray& ray, float& minT, glm::vec3& norm) const;
	virtual bool intersected(const Ray& ray) const;
	virtual void prepare_for_ray_tracing() = 0;
	virtual void Draw(Shader& shader) const = 0;
};

class Cube : public Object {
	static const float vertices[216];
	unsigned int VAO, VBO;
public:
	Cube() {}
	Cube(const Material& material0);
	void prepare_for_ray_tracing();
	void Draw(Shader& shader) const;
};

class Plane :public Object {
	static const float vertices[36];
	unsigned int VAO, VBO;

public:
	Plane() {}
	Plane(const Material& material0);
	void prepare_for_ray_tracing();
	void Draw(Shader& shader) const;
};

class Sphere : public Object {
	void find_minmax();
	glm::vec3 get_norm(const glm::vec3& p)const;
public:
	glm::vec3 center = glm::vec3(0.0f);
	float radius = 1.0f;

	Sphere() {}
	Sphere(const glm::vec3& center0, const float& radius0, const Material& material0);
	bool get_intersection(const Ray& ray, float& minT, glm::vec3& norm) const;
	bool intersected(const Ray& ray) const;
	void prepare_for_ray_tracing();
	void Draw(Shader& shader) const;
};
#endif