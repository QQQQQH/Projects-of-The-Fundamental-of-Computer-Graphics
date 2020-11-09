#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Face {
public:
	glm::vec3 points[3];
	glm::vec3 norm;
	bool in_face(const glm::vec3& p) const;
};

