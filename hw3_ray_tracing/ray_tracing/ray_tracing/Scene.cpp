#include "Scene.h"

const float Scene::INF = 100000000.0f, Scene::EPS = 1e-5;

bool Scene::get_intersection(const Ray& ray, const Object*& collidedObject, glm::vec3& collidedPoint, glm::vec3& norm, bool& inObject) {
	float minT = INF;
	bool intersect = false;
	for (const auto object : objects) {
		if (speedUp) {
			if (!object->intersect_AABB(ray)) {
				continue;
			}
		}
		bool intersect = object->get_intersection(ray, minT, norm, inObject);
		if (intersect) {
			collidedObject = object;
		}
	}
	if (collidedObject) {
		collidedPoint = ray.point_at_t(minT);
		intersect = true;
	}
	return intersect;
}

bool Scene::intersected(const Ray& ray) const {
	for (const auto object : objects) {
		if (speedUp) {
			if (!object->intersect_AABB(ray)) {
				continue;
			}
		}
		bool intersect = object->intersected(ray);
		if (intersect) {
			return true;
		}
	}
	return false;
}

glm::vec3 Scene::shade(const Object& object, const glm::vec3& fragPos, const glm::vec3& norm, const Ray& ray) {
	glm::vec3 ambient = ambientColor * object.ambient(fragPos);
	float distance = glm::length(lightPos - fragPos);
	float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

	if (intersected(Ray(fragPos, lightPos))) {
		ambient *= attenuation;
		return ambient;
		//return glm::vec3(1.0f, 0.0f, 0.0f);
	}

	glm::vec3 lightDir = glm::normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	glm::vec3 diffuse = diff * diffuseColor * object.diffuse(fragPos);

	glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
	float spec = glm::pow(max(glm::dot(-ray.dir, reflectDir), 0.0f), object.shininess());
	glm::vec3 specular = specularStrength * spec * object.specular(fragPos);


	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

void Scene::set_light_pos(const glm::vec3& lightPos0) {
	lightPos = lightPos0;
}

void Scene::set_speedUp(bool speedUp0) {
	speedUp = speedUp0;
}

void Scene::add_object(Object* const object) {
	objects.push_back(object);
}


void Scene::prepare_for_ray_tracing() {
	for (auto& object : objects) {
		object->prepare_for_ray_tracing();
	}
}

glm::vec3 Scene::trace_ray(const Ray& ray, unsigned int recursionStep) {
	glm::vec3 lightIntensity(0.0f); // 用于返回的光线强度，初始化为0

	// 递归结束条件：超过最大递归次数
	if (recursionStep >= MAX_RECURSION_STEP) {
		return lightIntensity;
	}

	// 计算光线与物体的交点以及该物体
	const Object* collidedObject = nullptr;
	glm::vec3 collidedPoint, norm;
	bool inObject = false;

	// 递归结束条件：光线没有照射到物体上
	if (!get_intersection(ray, collidedObject, collidedPoint, norm, inObject)) {
		return lightIntensity;
	}

	if (inObject) {
		// 若光线是从物体内部射出的，法向量应该取反
		norm = -norm;
	}

	// 光照强度的第一部分：局部光照强度
	if (!inObject && collidedObject->kShade() > EPS) {
		lightIntensity = collidedObject->kShade() *
			shade(*collidedObject, collidedPoint, norm, ray);
	}

	// 光照强度的第二部分：反射光照强度
	if (collidedObject->kReflect() > EPS) { // > 0
		// 计算反射方向
		glm::vec3 reflectDirection = glm::reflect(ray.dir, norm);
		lightIntensity += collidedObject->kReflect() *
			trace_ray(Ray(collidedPoint, collidedPoint + reflectDirection), recursionStep + 1);
	}

	// 光照强度的第三部分：折射光照强度
	if (collidedObject->kRefract() > EPS) { // > 0
		// 计算折射率
		float currentIndex = 1.0f;
		float nextIndex = collidedObject->refractiveIndex();
		if (inObject) {
			// 若光线是从物体内部射出的，折射率需要进行交换
			std::swap(currentIndex, nextIndex);
		}

		// 计算折射方向
		glm::vec3 refractDirection = glm::refract(ray.dir, norm, currentIndex / nextIndex);

		if (!isnan(refractDirection.x)) {
			lightIntensity += collidedObject->kRefract() *
				trace_ray(Ray(collidedPoint, collidedPoint + refractDirection), recursionStep + 1);
		}
	}
	return lightIntensity;
}




