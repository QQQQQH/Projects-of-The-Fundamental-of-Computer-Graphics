#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Object.h"
#include "Ray.h"

using namespace std;

class Scene {
	static const int MAX_RECURSION_STEP = 5;
	static const float INF, EPS;
	int cnt = 0;
	bool speedUp = false;

	bool get_intersection(const Ray& ray, const Object*& collidedObject, glm::vec3& collidedPoint, glm::vec3& norm, bool& inObject);
	bool intersected(const Ray& ray) const;
	glm::vec3 shade(const Object& object, const glm::vec3& pos, const glm::vec3& norm, const Ray& ray);
public:
	glm::vec3
		lightPos = glm::vec3(0.0f, 50.0f, 0.0f),
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f),
		diffuseColor = lightColor * glm::vec3(0.8f),
		ambientColor = lightColor * glm::vec3(0.4f),
		specularStrength = glm::vec3(1.0f, 1.0f, 1.0f);

	float
		constant = 1.0f,
		linear = 0.014f,
		quadratic = 0.0007f;

	vector<Object*> objects;

	void set_light_pos(const glm::vec3& lightPos0);
	void set_speedUp(bool speedUp0);
	void add_object(Object* const object);
	void prepare_for_ray_tracing();
	void setup_gl();
	glm::vec3 trace_ray(const Ray& ray, unsigned int recursionStep);
};



