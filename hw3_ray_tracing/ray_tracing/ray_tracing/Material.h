#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material {
public:
	const glm::vec3
		ambient = glm::vec3(1.0f),
		diffuse = ambient,
		specular = glm::vec3(0.5f) * ambient;
	const float
		shininess = 32.0f,
		kShade = 1.0f,
		kReflect = 0.0f,
		kRefract = 0.0f,
		refractiveIndex = 1.5f;

	Material() {}
	Material(const glm::vec3& color, const float& shininess0, const float& kShade0, const float& kReflect0, const float& kRefract0, const float& refractiveIndex0 = 1.5f) :
		ambient(color), diffuse(color), specular(color* glm::vec3(0.5f)), shininess(shininess0),
		kShade(kShade0), kReflect(kReflect0), kRefract(kRefract0), refractiveIndex(refractiveIndex0) {}
};

