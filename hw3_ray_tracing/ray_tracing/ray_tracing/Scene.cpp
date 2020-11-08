#include "Scene.h"

const float Scene::INF = 100000000.0f;
const float Scene::EPS = 1e-5;

tuple<const glm::vec3&, const Object*, const glm::vec3&> Scene::get_intersection(const Ray& ray) {
	float minT = INF;
	const Object* collidedObject = nullptr;
	const Face* collidedFace = nullptr;
	glm::vec3 start = ray.vertex, direction = ray.direction;
	glm::vec3 norm, collidedPoint;
	for (auto object : objects) {
		const Model* const model = object->model;
		for (const auto& mesh : model->meshes) {
			for (const auto& face : mesh.faces) {
				float v1 = glm::dot(face.points[0] - start, face.norm);
				float v2 = glm::dot(face.norm, direction);
				float t = v1 / v2;
				if (abs(v2) > EPS && v1 / v2 > EPS && t < minT) { // v2 != 0 && t >= 0 && t < minT
					glm::vec3 p = ray.point_at_t(t);
					if (face.in_face(p)) {
						minT = t;
						collidedObject = object;
						collidedFace = &face;
						collidedPoint = p;
					}
				}
			}
		}
	}
	if (collidedFace) {
		norm = collidedFace->norm;
	}
	return make_tuple(collidedPoint, collidedObject, norm);
}

glm::vec3 Scene::shade(const Object& object, const glm::vec3& pos, const glm::vec3& norm, const Ray& ray) {
	glm::vec3 ambient = ambientColor * object.ambient;

	glm::vec3 lightDir = glm::normalize(lightPos - pos);
	float diff = std::max(dot(norm, lightDir), 0.0f);
	glm::vec3 diffuse = diff * diffuseColor * object.diffuse;

	glm::vec3 middle = glm::normalize(-ray.direction + lightDir);
	float spec = glm::pow(std::max(glm::dot(middle, norm), 0.0f), object.shininess);
	glm::vec3 specular = specularStrength * spec * object.specular;

	return (ambient + diffuse + specular);
}

void Scene::add_object(Object* const object) {
	objects.push_back(object);
}


void Scene::prepare_for_ray_tracing() {
	for (auto& object : objects) {
		for (auto& mesh : object->model->meshes) {
			mesh.applyModel();
		}
	}
}

glm::vec3 Scene::trace_ray(const Ray& ray, unsigned int recursionStep) {
	glm::vec3 lightIntensity(0.0f); // ���ڷ��صĹ���ǿ�ȣ���ʼ��Ϊ0

	// �ݹ�����������������ݹ����
	if (recursionStep >= MAX_RECURSION_STEP) {
		return lightIntensity;
	}

	// �������������Ľ����Լ�������
	const auto& intersect_res = get_intersection(ray);
	const Object* collidedObject = get<1>(intersect_res);

	// �ݹ��������������û�����䵽������
	if (collidedObject == nullptr) {
		return lightIntensity;
	}

	// �������㼰�䷨����
	glm::vec3 collidedPoint = get<0>(intersect_res);
	glm::vec3 normal = get<2>(intersect_res);
	bool inObject = glm::dot(ray.direction, normal) > 0;

	if (inObject) {
		// �������Ǵ������ڲ�����ģ�������Ӧ��ȡ��
		normal = -normal;
	}

	// ����ǿ�ȵĵ�һ���֣��ֲ�����ǿ��
	if (!inObject) {
		lightIntensity = collidedObject->kShade *
			shade(*collidedObject, collidedPoint, normal, ray);
	}


	// ���㷴�䷽��
	glm::vec3 reflectDirection = glm::reflect(ray.direction, normal);

	// ����ǿ�ȵĵڶ����֣��������ǿ��
	if (collidedObject->kReflect > EPS) { // > 0
		lightIntensity += collidedObject->kReflect *
			trace_ray(Ray(collidedPoint, collidedPoint + reflectDirection), recursionStep + 1);
	}

	// ����������
	float currentIndex = 1.0f;
	float nextIndex = collidedObject->refractiveIndex;
	if (inObject) {
		// �������Ǵ������ڲ�����ģ���������Ҫ���н���
		std::swap(currentIndex, nextIndex);
	}

	// �������䷽��
	glm::vec3 refractDirection = glm::refract(ray.direction, normal, currentIndex / nextIndex);

	// ����ǿ�ȵĵ������֣��������ǿ��
	if (collidedObject->kRefract > EPS) { // > 0
		lightIntensity += collidedObject->kRefract *
			trace_ray(Ray(collidedPoint, collidedPoint + refractDirection), recursionStep + 1);
	}

	return lightIntensity;
}




