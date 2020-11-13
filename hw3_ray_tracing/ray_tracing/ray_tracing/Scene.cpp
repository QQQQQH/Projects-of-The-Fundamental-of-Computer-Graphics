#include "Scene.h"

const float Scene::INF = 100000000.0f, Scene::EPS = 1e-7;

bool Scene::get_intersection(const Ray& ray, const Object*& collidedObject, glm::vec3& collidedPoint, glm::vec3& norm) {
	float minT = INF;
	bool intersect = false;
	for (const auto object : objects) {
		if (speedUp) {
			if (!object->intersect_AABB(ray)) {
				continue;
			}
		}
		bool intersect = object->get_intersection(ray, minT, norm);
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

glm::vec3 Scene::shade(const Object& object, const glm::vec3& pos, const glm::vec3& norm, const Ray& ray) {
	glm::vec3 ambient = ambientColor * object.ambient();

	if (intersected(Ray(pos, lightPos))) {
		//return ambient;
	}

	glm::vec3 lightDir = glm::normalize(lightPos - pos);
	float diff = max(dot(norm, lightDir), 0.0f);
	glm::vec3 diffuse = diff * diffuseColor * object.diffuse();

	glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
	float spec = glm::pow(max(glm::dot(-ray.dir, reflectDir), 0.0f), object.shininess());
	glm::vec3 specular = specularStrength * spec * object.specular();

	return (ambient + diffuse + specular);
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
	glm::vec3 lightIntensity(0.0f); // ���ڷ��صĹ���ǿ�ȣ���ʼ��Ϊ0

	// �ݹ�����������������ݹ����
	if (recursionStep >= MAX_RECURSION_STEP) {
		return lightIntensity;
	}

	// �������������Ľ����Լ�������
	const Object* collidedObject = nullptr;
	glm::vec3 collidedPoint, norm;

	// �ݹ��������������û�����䵽������
	if (!get_intersection(ray, collidedObject, collidedPoint, norm)) {
		return lightIntensity;
	}

	bool inObject = glm::dot(ray.dir, norm) > 0;

	if (inObject) {
		// �������Ǵ������ڲ�����ģ�������Ӧ��ȡ��
		norm = -norm;
	}

	// ����ǿ�ȵĵ�һ���֣��ֲ�����ǿ��
	if (!inObject) {
		lightIntensity = collidedObject->kShade() *
			shade(*collidedObject, collidedPoint, norm, ray);
	}

	// ����ǿ�ȵĵڶ����֣��������ǿ��
	if (collidedObject->kReflect() > EPS) { // > 0
		// ���㷴�䷽��
		glm::vec3 reflectDirection = glm::reflect(ray.dir, norm);
		lightIntensity += collidedObject->kReflect() *
			trace_ray(Ray(collidedPoint, collidedPoint + reflectDirection), recursionStep + 1);
	}

	// ����ǿ�ȵĵ������֣��������ǿ��
	if (collidedObject->kRefract() > EPS) { // > 0
		// ����������
		float currentIndex = 1.0f;
		float nextIndex = collidedObject->refractiveIndex();
		if (inObject) {
			// �������Ǵ������ڲ�����ģ���������Ҫ���н���
			std::swap(currentIndex, nextIndex);
		}

		// �������䷽��
		glm::vec3 refractDirection = glm::refract(ray.dir, norm, currentIndex / nextIndex);

		lightIntensity += collidedObject->kRefract() *
			trace_ray(Ray(collidedPoint, collidedPoint + refractDirection), recursionStep + 1);
	}
	return lightIntensity;
}




