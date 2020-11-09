#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
using namespace std;

ostream& operator<<(ostream& out, const glm::vec3& v);

class Face {
public:
	glm::vec3 points[3];
	glm::vec3 norm;
	bool on_face(const glm::vec3& P) const;
};

