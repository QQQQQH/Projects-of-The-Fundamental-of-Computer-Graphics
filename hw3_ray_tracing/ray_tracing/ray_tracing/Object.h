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
	glm::vec3 maxv = glm::vec3(-INF), minv = glm::vec3(INF);

	virtual void find_minmax();
public:
	int numOfTriangles = 0;
	Material material;
	glm::mat4 model = glm::mat4(1.0f);
	vector<Face> faces;

	Object(const Material& material0) :material(material0) {}

	virtual glm::vec3 ambient(const glm::vec3& p) const { return material.ambient; }
	virtual glm::vec3 diffuse(const glm::vec3& p) const { return material.diffuse; }
	virtual glm::vec3 specular(const glm::vec3& p) const { return material.specular; }
	glm::vec3 ambient() const { return material.ambient; }
	glm::vec3 diffuse() const { return material.diffuse; }
	glm::vec3 specular() const { return material.specular; }
	float shininess() const { return material.shininess; }
	float kShade() const { return material.kShade; }
	float kReflect() const { return material.kReflect; }
	float kRefract() const { return material.kRefract; }
	float refractiveIndex() const { return material.refractiveIndex; }

	void set_model(const glm::mat4& model0);
	bool intersect_AABB(const Ray& ray) const;

	virtual void setup_gl() {}
	virtual bool get_intersection(const Ray& ray, float& minT, glm::vec3& norm, bool& inObject) const;
	virtual bool intersected(const Ray& ray) const;
	virtual void prepare_for_ray_tracing() = 0;
	virtual void Draw(Shader& shader) const = 0;
};

class Cube : public Object {
	static const float vertices[216];
	unsigned int VAO, VBO;
public:
	Cube(const Material& material0) :Object(material0) {}

	void setup_gl();
	void prepare_for_ray_tracing();
	void Draw(Shader& shader) const;
};

class Plane :public Object {
	static const float vertices[36];
	unsigned int VAO, VBO;
	glm::vec3 white = glm::vec3(1.0f), black = glm::vec3(0.0f);

	bool is_white(const glm::vec3& p)const;
public:
	Plane(const Material& material0) : Object(material0) {}

	void setup_gl();
	void set_color(const glm::vec3& white0, const glm::vec3& black0) { white = white0; black = black0; }
	glm::vec3 ambient(const glm::vec3& p) const { return is_white(p) ? white : black; }
	glm::vec3 diffuse(const glm::vec3& p) const { return ambient(p); }
	glm::vec3 specular(const glm::vec3& p) const { return glm::vec3(0.5f) * ambient(p); }
	void prepare_for_ray_tracing();
	void Draw(Shader& shader) const;
};

class Sphere : public Object {
	void find_minmax();
	glm::vec3 get_norm(const glm::vec3& p)const;
public:
	glm::vec3 center = glm::vec3(0.0f);
	float radius = 1.0f;

	Sphere(const Material& material0, const glm::vec3& center0 = glm::vec3(0), const float& radius0 = 1);

	void set_center_radius(const glm::vec3& center0, const float& radius0 = 1) { center = center0, radius = radius0; }
	bool get_intersection(const Ray& ray, float& minT, glm::vec3& norm, bool& inObject) const;
	bool intersected(const Ray& ray) const;
	void prepare_for_ray_tracing();
	void Draw(Shader& shader) const;
};
#endif