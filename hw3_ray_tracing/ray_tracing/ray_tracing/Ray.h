#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Ray {
public:
	glm::vec3 vertex;
	glm::vec3 direction;

	Ray(glm::vec3 src, glm::vec3 dest) : vertex(src), direction(glm::normalize(dest - src)) {}
	glm::vec3 point_at_t(float t) const {
		return vertex + t * direction;
	}
};
