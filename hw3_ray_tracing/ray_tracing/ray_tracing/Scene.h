#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Object.h"
#include "Ray.h"

using namespace std;

class Scene {
	static const int MAX_RECURSION_STEP = 1;
	static const float INF, EPS;

	tuple<glm::vec3, const Object*, glm::vec3> get_intersection(const Ray& ray);
	glm::vec3 shade(const Object& object, const glm::vec3& pos, const glm::vec3& norm, const Ray& ray);
public:
	glm::vec3
		lightPos = glm::vec3(0.0f, 3.0f, 0.0f),
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f),
		diffuseColor = lightColor * glm::vec3(0.5f), // decrease the influence
		ambientColor = diffuseColor * glm::vec3(0.2f), // low influence
		specularStrength = glm::vec3(1.0f, 1.0f, 1.0f);

	vector<Object*> objects;
	void add_object(Object* const object);
	void prepare_for_ray_tracing();
	glm::vec3 trace_ray(const Ray& ray, unsigned int recursionStep = 0);
};



