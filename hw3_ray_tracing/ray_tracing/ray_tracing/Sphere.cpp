#include "Object.h"

void Sphere::find_minmax() {
	minv = center - glm::vec3(radius);
	maxv = center + glm::vec3(radius);
}

glm::vec3 Sphere::get_norm(const glm::vec3& p) const {
	return glm::normalize(p - center);
}

Sphere::Sphere(const glm::vec3& center0, const float& radius0, const Material& material0) :
	Object(material0), center(center0), radius(radius0) {}

bool Sphere::get_intersection(const Ray& ray, float& minT, glm::vec3& norm) const {
	glm::vec3 vc = ray.src - center;

	float A = 1;
	float B = 2 * glm::dot(vc, ray.dir);
	float C = glm::dot(vc, vc) - radius * radius;
	if (abs(C) < EPS) {
		C = 0;
	}

	float delta = B * B - 4 * A * C;
	if (delta < EPS) {
		return false;
	}
	delta = sqrt(delta);
	float t1 = (-B + delta) / 2 / A;
	float t2 = (-B - delta) / 2 / A;
	if (t1 < EPS && t2 < EPS) {
		return false;
	}
	if (t2 > EPS) {
		minT = t2;
		norm = get_norm(ray.point_at_t(t2));
	}
	else {
		minT = t1;
		norm = get_norm(ray.point_at_t(t1));
	}
	return true;
}

bool Sphere::intersected(const Ray& ray) const {
	glm::vec3 vc = ray.src - center;

	float A = 1;
	float B = 2 * glm::dot(vc, ray.dir);
	float C = glm::dot(vc, vc) - radius * radius;
	if (abs(C) < EPS) {
		C = 0;
	}

	float delta = B * B - 4 * A * C;
	if (delta < EPS) {
		return false;
	}
	delta = sqrt(delta);
	float t1 = (-B + delta) / 2 / A;
	float t2 = (-B - delta) / 2 / A;
	if (t1 < EPS && t2 < EPS) {
		return false;
	}
	return true;
}

void Sphere::prepare_for_ray_tracing() {
	find_minmax();
}

void Sphere::Draw(Shader& shader) const {}
