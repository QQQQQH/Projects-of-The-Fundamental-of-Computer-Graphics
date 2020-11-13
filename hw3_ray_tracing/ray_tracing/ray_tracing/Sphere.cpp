#include "Object.h"

void Sphere::find_minmax() {
	minv = center - glm::vec3(radius);
	maxv = center + glm::vec3(radius);
}

glm::vec3 Sphere::get_norm(const glm::vec3& p) const {
	return glm::normalize(p - center);
}

Sphere::Sphere(const Material& material0, const glm::vec3& center0, const float& radius0) :
	Object(material0), center(center0), radius(radius0) {}

bool Sphere::get_intersection(const Ray& ray, float& minT, glm::vec3& norm) const {
	glm::vec3
		src = ray.src,
		dir = ray.dir,
		vc = src - center,
		p;

	float A = 1;
	float B = 2 * glm::dot(vc, dir);
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
	if (t1 < EPS) {
		return false;
	}
	if (t2 > EPS && t2 < minT) {
		minT = t2;
		norm = get_norm(ray.point_at_t(t2));
		return true;
	}
	else if (t1 < minT) {
		minT = t1;
		norm = get_norm(ray.point_at_t(t1));
		return true;
	}
	return false;
}

bool Sphere::intersected(const Ray& ray) const {
	glm::vec3
		src = ray.src,
		dir = ray.dir,
		dest = ray.dest,
		vc = src - center,
		p;

	float A = 1;
	float B = 2 * glm::dot(vc, dir);
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
	if (t1 < EPS) {
		return false;
	}
	if (t2 > EPS) {
		p = ray.point_at_t(t2);
	}
	else {
		p = ray.point_at_t(t1);
	}
	return abs(p.x - src.x) < abs(dest.x - src.x);
}

void Sphere::prepare_for_ray_tracing() {
	find_minmax();
}

void Sphere::Draw(Shader& shader) const {}
