#include "Object.h"


Cube::Cube() {
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
