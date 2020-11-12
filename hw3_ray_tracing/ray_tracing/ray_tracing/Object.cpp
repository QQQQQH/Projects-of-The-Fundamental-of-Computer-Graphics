#include "Object.h"

const float Object::INF = 100000000.0f, Object::EPS = 1e-7;

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

	// x
	if (abs(dir.x) > EPS) {
		if (dir.x > 0) {
			t = (minv.x - src.x) / dir.x;
		}
		else {
			t = (maxv.x - src.x) / dir.x;
		}
		if (t > 0) {
			p = ray.point_at_t(t);
			if (minv.y <= p.y && maxv.y >= p.y && minv.z <= p.z && maxv.z >= p.z) {
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
		if (t > 0) {
			p = ray.point_at_t(t);
			if (minv.x <= p.x && maxv.x >= p.x && minv.z <= p.z && maxv.z >= p.z) {
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
		if (t > 0) {
			p = ray.point_at_t(t);
			if (minv.y <= p.y && maxv.y >= p.y && minv.x <= p.x && maxv.x >= p.x) {
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
			if (t > -EPS && t < minT) {
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

void Cube::set_up() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

Cube::Cube() {
	set_up();
}

void Cube::prepare_for_ray_tracing() {
	for (int i = 0; i < 12; ++i) {
		faces.push_back(Face());
		for (int j = 0; j < 3; ++j) {
			glm::vec4 t = model * glm::vec4(
				vertices[i * 18 + j * 6],
				vertices[i * 18 + j * 6 + 1],
				vertices[i * 18 + j * 6 + 2], 1.0f);
			faces[i].points[j] = glm::vec3(t.x, t.y, t.z);
			faces[i].norms[j] = glm::vec3(
				vertices[i * 18 + j * 6 + 3],
				vertices[i * 18 + j * 6 + 4],
				vertices[i * 18 + j * 6 + 5]);
		}
		auto
			ab = faces[i].points[1] - faces[i].points[0],
			ac = faces[i].points[2] - faces[i].points[0];
		faces[i].norm = glm::normalize(glm::cross(ab, ac));
	}
	find_minmax();
}

void Cube::Draw(Shader& shader) const {
	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


const float Cube::vertices[216] = {
	// front
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	// back
	 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
	 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,

	 // left
	 -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	 -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
	 -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
	 -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
	 -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	 -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

	 // right
	 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,

	 // top
	 -1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
	  1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
	  1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	  1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	 -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	 -1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f,

	 // down
	 -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
	  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
	  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f,
	  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f,
	 -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f,
	 -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
};

