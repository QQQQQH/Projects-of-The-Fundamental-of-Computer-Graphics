#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <tuple>
using namespace std;

ostream& operator<<(ostream& out, const glm::vec3& v);

class Face {
	static const float EPS;
public:
	glm::vec3 points[3];
	glm::vec3 norms[3];
	glm::vec3 norm;
	bool on_face(const glm::vec3& P) const;
	bool on_face(const glm::vec3& P, float& u, float& v) const;
	glm::vec3 cal_norm(float u, float v)const;
};

