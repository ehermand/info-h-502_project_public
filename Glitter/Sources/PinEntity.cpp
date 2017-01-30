#include "PinEntity.hpp"

#include <glm/gtc/matrix_transform.hpp>

PinEntity::PinEntity(Model *model, glm::vec3 position, glm::vec3 orientation, glm::vec3 scale) :
	Entity(model, position, orientation, scale) {
}

void PinEntity::updatePose(glm::mat4 modelMatrix) {
	mModelMatrix = modelMatrix;
	// Correct scale and offset from bullet
	mModelMatrix = glm::translate(mModelMatrix, glm::vec3(0.0f, -0.381f / 2 + 0.381f / 6, 0.0f));
	mModelMatrix = glm::scale(mModelMatrix, mScale);

	mPosition = glm::vec3(mModelMatrix[3]);
}