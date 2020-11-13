#include "Material.h"

void Material::set_color(const glm::vec3& color) {
	ambient = color;
	diffuse = color;
	specular = glm::vec3(0.5f) * color;
	shininess = 32.0f;
}

void Material::set_pure_color() {
	kShade = 1.0f;
	kReflect = 0.0f;
	kRefract = 0.0f;
}

void Material::set_metal() {
	kShade = 0.9f;
	kReflect = 0.1f;
	kRefract = 0.0f;
}

void Material::set_smooth() {
	kShade = 0.6f;
	kReflect = 0.4f;
	kRefract = 0.0f;
}

void Material::set_full_reflect() {
	kShade = 0.0f;
	kReflect = 1.0f;
	kRefract = 0.0f;
}

void Material::set_full_refract() {
	kShade = 0.0f;
	kReflect = 0.0f;
	kRefract = 1.0f;
}

void Material::set_gold() {
	//ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
	//diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
	//specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
	//shininess = 0.4 * 128;

	ambient = glm::vec3(0.2125, 0.1275, 0.054);
	diffuse = glm::vec3(0.714, 0.4284, 0.18144);
	specular = glm::vec3(0.393548, 0.271906, 0.166721);
	shininess = 0.2 * 128;
}
