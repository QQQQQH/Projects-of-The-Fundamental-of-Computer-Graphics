#include "Object.h"

bool Plane::is_white(const glm::vec3& p) const {
	return fmod(floor(p.x * 2) + floor(p.z * 2), 2);
	//return true;
}

Plane::Plane(const Material& material0) :Object(material0) {
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

void Plane::prepare_for_ray_tracing() {
	for (int i = 0; i < 2; ++i) {
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

void Plane::Draw(Shader& shader) const {
	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

const float Plane::vertices[36] = {
	// top
	-1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
};
