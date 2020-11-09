#include "Object.h"



void Object::set_model(const glm::mat4& model0) {
	model = model0;
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
		cout << "---------" << endl;
		for (int j = 0; j < 3; ++j) {
/*			glm::vec4 t = model * glm::vec4(
				vertices[i * 18 + j * 6],
				vertices[i * 18 + j * 6 + 1],
				vertices[i * 18 + j * 6 + 2], 1.0f);	*/		
			glm::vec4 t = model * glm::vec4(
					vertices[i * 9 + j * 3],
					vertices[i * 9 + j * 3 + 1],
					vertices[i * 9 + j * 3 + 2], 1.0f);
			faces[i].points[j] = glm::vec3(t.x, t.y, t.z);
			cout << faces[i].points[j];
		}
		auto
			ab = faces[i].points[1] - faces[i].points[0],
			ac = faces[i].points[2] - faces[i].points[0];
		faces[i].norm = glm::normalize(glm::cross(ab, ac));
		cout << faces[i].norm;
	}
}

tuple<float, glm::vec3> Cube::get_intersection(const Ray& ray) {
	float minT = INF;
	const Face* collidedFace = nullptr;
	glm::vec3 start = ray.vertex, direction = ray.direction;
	glm::vec3 norm;
	for (const auto& face : faces) {
		float v1 = glm::dot(face.norm, face.points[0] - start);
		float v2 = glm::dot(face.norm, direction);
		if (abs(v2) > EPS) { // v2 != 0
			float t = v1 / v2;
			if (t > -EPS && t < minT) {
				glm::vec3 p = ray.point_at_t(t);
				if (face.on_face(p)) {
					minT = t;
					collidedFace = &face;
				}
			}
		}
	}
	if (collidedFace) {
		norm = collidedFace->norm;
	}
	return make_tuple(minT, norm);
}

void Cube::Draw(Shader& shader) {
	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

const float Cube::vertices[108] = {
	// front
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	// back
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	// left
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	// right
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,

	// top
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,

	// down
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f,
};

// Sphere
Sphere::Sphere(const glm::vec3& center, float radius) : _center(center), _radius(radius) {

}

bool Sphere::inSphere(const glm::vec3& p) const {
	return glm::distance(p, _center) < _radius + EPS;
}

float Sphere::rayCollision(const Ray& ray) const {
	glm::vec3 vc = ray.vertex - _center;

	float A = glm::dot(ray.direction, ray.direction);
	float B = 2 * glm::dot(vc, ray.direction);
	float C = glm::dot(vc, vc) - _radius * _radius;
	if (abs(C) < EPS) {
		C = 0;
	}

	float delta = B * B - 4 * A * C;
	if (delta < EPS) {
		return -1;
	}
	delta = sqrt(delta);
	float t1 = (-B + delta) / 2 / A;
	float t2 = (-B - delta) / 2 / A;
	if (t1 < EPS&& t2 < EPS) {
		return -1;
	}
	if (t2 > EPS) {
		return t2;
	}
	else {
		return t1;
	}
}

glm::vec3 Sphere::calNormal(const glm::vec3& p) const {
	return glm::normalize(p - _center);
}
bool Sphere::rayInEntity(const Ray& ray) const {
	return inSphere(ray.vertex) && rayCollision(ray) > EPS;
}
