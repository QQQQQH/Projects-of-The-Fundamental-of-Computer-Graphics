#include "Face.h"

ostream& operator<<(ostream& out, const glm::vec3& v) {
	out << "(" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
	return out;
}

bool Face::on_face(const glm::vec3& P) const {
	const glm::vec3
		& A = points[0],
		& B = points[1],
		& C = points[2];

	glm::vec3
		v0 = C - A,
		v1 = B - A,
		v2 = P - A;

	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0 || u > 1) {
		return false;
	}

	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0 || v > 1) {
		return false;
	}
	return u + v <= 1;

}



bool Face::on_face(const glm::vec3& P, float& u, float& v) const {
	const glm::vec3
		& A = points[0],
		& B = points[1],
		& C = points[2];

	glm::vec3
		v0 = C - A,
		v1 = B - A,
		v2 = P - A;

	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

	u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0 || u > 1) {
		return false;
	}

	v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0 || v > 1) {
		return false;
	}
	return u + v <= 1;

}

glm::vec3 Face::cal_norm(float u, float v) const {
	return (1 - u - v) * norms[0] + u * norms[1] + v * norms[2];
}
