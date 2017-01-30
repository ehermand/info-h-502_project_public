#include "Entity.hpp"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(Model *model, glm::vec3 position, glm::vec3 orientation, glm::vec3 scale) : mModel(model) {
	mPosition = position;
	mOrientation = orientation;
	mScale = scale;
	updateModelMatrix();
	// Save default state
	mDefaultModelMatrix = mModelMatrix;
}

void Entity::updatePose(btVector3 position, btQuaternion orientation) {
	mModelMatrix = glm::mat4(1.0f);
	mModelMatrix = glm::translate(mModelMatrix, glm::vec3(position.getX(), position.getY(), position.getZ()));
	mModelMatrix = glm::rotate(mModelMatrix, orientation.getAngle(), glm::vec3(orientation.getAxis().getX(), orientation.getAxis().getY(), orientation.getAxis().getZ()));
	mModelMatrix = glm::scale(mModelMatrix, mScale);
}
void Entity::updatePose(glm::mat4 modelMatrix) {
	mModelMatrix = modelMatrix;
	mModelMatrix = glm::scale(mModelMatrix, mScale);
}

void Entity::updateModelMatrix() {
	mModelMatrix = glm::mat4(1.0f);
	mModelMatrix = glm::translate(mModelMatrix, mPosition);
	mModelMatrix = glm::rotate(mModelMatrix, glm::radians(mOrientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mModelMatrix = glm::rotate(mModelMatrix, glm::radians(mOrientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mModelMatrix = glm::rotate(mModelMatrix, glm::radians(mOrientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	mModelMatrix = glm::scale(mModelMatrix, mScale);
}

void Entity::draw(Shader &shader) {
	mModel->draw(shader);
}
