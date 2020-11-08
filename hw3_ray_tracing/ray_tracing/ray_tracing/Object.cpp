#include "Object.h"

// -------------------------------Object-------------------------------
void Object::set_vertices_data() {
	glBufferData(GL_ARRAY_BUFFER, get_vertices_size(), get_vertices(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Object::find_minmax() {
	min = glm::vec3(INF, INF, INF), max = glm::vec3(-INF, -INF, -INF);
	int verticesNum = get_vertices_num();
	const float* vertices = get_vertices();
	for (int i = 0; i < verticesNum; ++i) {
		int k = i * 5;
		glm::vec3 t = get_model() * glm::vec4(vertices[k], vertices[k + 1], vertices[k + 2], 1);

		if (t.x < min.x) {
			min.x = t.x;
		}
		else if (t.x > max.x) {
			max.x = t.x;
		}
		if (t.y < min.y) {
			min.y = t.y;
		}
		else if (t.y > max.y) {
			max.y = t.y;
		}
		if (t.z < min.z) {
			min.z = t.z;
		}
		else if (t.z > max.z) {
			max.z = t.z;
		}
	}
}

void Object::set_pos(glm::vec3 pos0) {
	pos = pos0;
}

void Object::set_rotate(float rotate10) {
	rotate1 = rotate10;
}

void Object::set_rotate(float rotate10, glm::vec3 roatate20) {
	rotate1 = rotate10;
	rotate2 = roatate20;
}

glm::mat4 Object::get_model() {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, pos);
	trans = glm::scale(trans, scale);
	trans = glm::rotate(trans, rotate1, rotate2);
	return trans;
}

bool Object::check_hit(const Object& obj) {
	return ((min.x >= obj.min.x && min.x <= obj.max.x) || (obj.min.x >= min.x && obj.min.x <= max.x)) &&
		((min.y >= obj.min.y && min.y <= obj.max.y) || (obj.min.y >= min.y && obj.min.y <= max.y)) &&
		((min.z >= obj.min.z && min.z <= obj.max.z) || (obj.min.z >= min.z && obj.min.z <= max.z));
}


// -------------------------------Cube-------------------------------
const float Cube::vertices[180] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
int Cube::get_vertices_num() {
	return verticesNum;
}
int Cube::get_vertices_size() {
	return verticesSize;
}

const float* Cube::get_vertices() {
	return vertices;
}
