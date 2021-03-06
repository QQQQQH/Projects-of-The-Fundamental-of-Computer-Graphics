#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Ray {
public:
	glm::vec3 src, dest, dir;

	Ray(const glm::vec3& src0, const glm::vec3& dest0) : src(src0), dest(dest0), dir(glm::normalize(dest - src0)) {}
	glm::vec3 point_at_t(float t) const {
		return src + t * dir;
	}
};
