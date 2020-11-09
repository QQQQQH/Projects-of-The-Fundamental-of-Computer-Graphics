#include "Face.h"

bool Face::in_face(const glm::vec3& p) const {
	if (glm::dot(p - points[0], norm)) {
		return false;
	}

	glm::vec3 vectorToP[3];
	glm::vec3 crosses[3];
	for (int i = 0; i < 3; i++) {
		vectorToP[i] = p - points[i];
	}

	const float EPS = 1e-5;

	for (int i = 0; i < 3; i++) {
		crosses[i] = glm::normalize(glm::cross(vectorToP[i], vectorToP[(i + 1) % 3]));
		if (crosses[i].x < EPS &&
			crosses[i].y < EPS &&
			crosses[i].z < EPS) {
			return false;
		}
	}

	if (
		glm::distance(crosses[0], crosses[1]) > EPS ||
		glm::distance(crosses[1], crosses[2]) > EPS) {
		return false;
	}
	return true;
}