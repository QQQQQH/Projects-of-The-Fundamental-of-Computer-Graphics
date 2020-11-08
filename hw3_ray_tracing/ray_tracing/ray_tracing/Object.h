#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object {
public:
	static const int INF = 0x7fffffff;

	glm::vec3 pos, scale;
	float rotate1 = 0;
	glm::vec3 rotate2 = glm::vec3(0, 0, 1);
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 min = glm::vec3(INF, INF, INF), max = glm::vec3(-INF, -INF, -INF);

	Object(glm::vec3 pos0 = glm::vec3(0, 0, 0),
		glm::vec3 scale0 = glm::vec3(1, 1, 1)) :pos(pos0), scale(scale0) {}

	virtual int get_vertices_num() = 0;
	virtual int get_vertices_size() = 0;
	virtual const float* get_vertices() = 0;
	void set_vertices_data();
	void find_minmax();
	void set_pos(glm::vec3 pos0);
	void set_rotate(float rotate10);
	void set_rotate(float rotate10, glm::vec3 roatate20);
	glm::mat4 get_model();
	bool check_hit(const Object& obj);
};

class Cube : public Object {
public:
	const static int verticesNum = 36;
	const static float vertices[180];
	const static int verticesSize = sizeof(vertices);

	Cube(glm::vec3 pos0 = glm::vec3(0, 0, 0),
		glm::vec3 scale0 = glm::vec3(1, 1, 1)) :Object(pos0, scale0) {}

	virtual int get_vertices_num();
	virtual int get_vertices_size();
	virtual const float* get_vertices();
};
#endif