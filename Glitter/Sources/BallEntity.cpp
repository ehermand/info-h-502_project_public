#include "BallEntity.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

BallEntity::BallEntity(Model *model, glm::vec3 position, glm::vec3 orientation, glm::vec3 scale) :
	Entity(model, position, orientation, scale) {
}

void BallEntity::updatePose(glm::mat4 modelMatrix) {
	mModelMatrix = modelMatrix;
	mModelMatrix = glm::scale(mModelMatrix, mScale);

	mPosition = glm::vec3(mModelMatrix[3]);
}