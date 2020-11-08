#include "Object.h"

void Object::set_model_trans(glm::mat4 modelTrans0) {
	modelTrans = modelTrans0;
	for (auto& mesh : model->meshes) {
		mesh.model = modelTrans;
	}
}
