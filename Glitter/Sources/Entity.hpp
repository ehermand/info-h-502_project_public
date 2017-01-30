#pragma once

#include<glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include "Model.hpp"

class Entity {
public:
	Entity(Model *model, glm::vec3 position, glm::vec3 orientation, glm::vec3 scale = glm::vec3(1.0f));

	void updatePose(btVector3 position, btQuaternion orientation);
	virtual void updatePose(glm::mat4 modelMatrix);
	void updateModelMatrix();

	void draw(Shader &shader);

	glm::mat4 getModelMatrix() { return mModelMatrix; }
	glm::mat4 getDefaultModelMatrix() { return mDefaultModelMatrix; }
	glm::vec3 getPosition() { return mPosition; }

protected:
	Model *mModel;
	glm::mat4 mModelMatrix;
	glm::mat4 mDefaultModelMatrix;
	glm::vec3 mPosition;
	glm::vec3 mOrientation; // Euler angles
	glm::vec3 mScale;
};
