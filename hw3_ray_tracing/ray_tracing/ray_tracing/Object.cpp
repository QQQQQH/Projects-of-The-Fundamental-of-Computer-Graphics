#include "Object.h"

const float Object::INF = 100000000.0f, Object::EPS = 1e-5;

void Object::find_minmax() {
	for (const auto& face : faces) {
		for (const auto& p : face.points) {
			if (p.x < minv.x) {
				minv.x = p.x;
			}
			else if (p.x > maxv.x) {
				maxv.x = p.x;
			}
			if (p.y < minv.y) {
				minv.y = p.y;
			}
			else if (p.y > maxv.y) {
				maxv.y = p.y;
			}
			if (p.z < minv.z) {
				minv.z = p.z;
			}
			else if (p.z > maxv.z) {
				maxv.z = p.z;
			}
		}
	}
}

bool Object::intersect_AABB(const Ray& ray) const {
	float t;
	glm::vec3 p;
	const glm::vec3& src = ray.src, & dir = ray.dir;
	float
		minx = minv.x - EPS,
		miny = minv.y - EPS,
		minz = minv.z - EPS,
		maxx = maxv.x + EPS,
		maxy = maxv.y + EPS,
		maxz = maxv.z + EPS;

	// x
	if (abs(dir.x) > EPS) {
		if (dir.x > 0) {
			t = (minv.x - src.x) / dir.x;
		}
		else {
			t = (maxv.x - src.x) / dir.x;
		}
		if (t >= 0) {
			p = ray.point_at_t(t);
			if (miny <= p.y && maxy >= p.y && minz <= p.z && maxz >= p.z) {
				return true;
			}
		}
	}

	// y
	if (abs(dir.y) > EPS) {
		if (dir.y > 0) {
			t = (minv.y - src.y) / dir.y;
		}
		else {
			t = (maxv.y - src.y) / dir.y;
		}
		if (t >= 0) {
			p = ray.point_at_t(t);
			if (minx <= p.x && maxx >= p.x && minz <= p.z && maxz >= p.z) {
				return true;
			}
		}
	}

	// z
	if (abs(dir.z) > EPS) {
		if (dir.z > 0) {
			t = (minv.z - src.z) / dir.z;
		}
		else {
			t = (maxv.z - src.z) / dir.z;
		}
		if (t >= 0) {
			p = ray.point_at_t(t);
			if (miny <= p.y && maxy >= p.y && minx <= p.x && maxx >= p.x) {
				return true;
			}
		}
	}
	return false;
}

void Object::set_model(const glm::mat4& model0) {
	model = model0;
}

bool Object::get_intersection(const Ray& ray, float& minT, glm::vec3& norm) {
	float t, u, v, v1, v2;
	glm::vec3 src = ray.src, dir = ray.dir, p;
	bool intersect = false;
	for (const auto& face : faces) {
		v1 = glm::dot(face.norm, face.points[0] - src);
		v2 = glm::dot(face.norm, dir);
		if (abs(v2) > EPS) { // v2 != 0
			t = v1 / v2;
			if (t > EPS && t < minT) {
				p = ray.point_at_t(t);
				bool onFace = face.on_face(p, u, v);
				//bool onFace = face.on_face(p);
				if (onFace) {
					minT = t;
					norm = face.norm;
					//norm = face.cal_norm(u, v);
					intersect = true;
				}
			}
		}
	}
	return intersect;
}

bool Object::intersected(const Ray& ray) const {
	float t, v1, v2;
	glm::vec3 src = ray.src, dir = ray.dir, p;
	for (const auto& face : faces) {
		v1 = glm::dot(face.norm, face.points[0] - src);
		v2 = glm::dot(face.norm, dir);
		if (abs(v2) > EPS) { // v2 != 0
			t = v1 / v2;
			if (t > EPS) {
				p = ray.point_at_t(t);
				bool onFace = face.on_face(p);
				if (onFace) {
					return true;
				}
			}
		}
	}
	return false;
}

