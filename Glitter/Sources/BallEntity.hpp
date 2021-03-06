#pragma once

#include<glm/glm.hpp>

#include "Entity.hpp"
#include "Model.hpp"

class BallEntity: public Entity {
public:
	BallEntity(Model *model, glm::vec3 position, glm::vec3 orientation, glm::vec3 scale = glm::vec3(1.0f));

	void updatePose(glm::mat4 modelMatrix);
};
