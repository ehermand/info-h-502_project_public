#pragma once

#include <glad/glad.h>
#include<glm/glm.hpp>

#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Entity.hpp"

struct PointLight {
	glm::vec3 position;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};

class Renderer {
public:
	Renderer();

	void setCurrentCamera(Camera *camera);
	void setSkybox(Skybox *skybox, Shader *skyboxShader);
	void setEntityShader(Shader *shader);
	void setAspectRatio(GLfloat);

	void clear();
	void updateMatrices();
	void drawSkybox();
	void drawEntity(Entity *entity);
	void drawEntity(Entity *entity, Shader *shader);
	void drawLights(Model *lightModel, Shader *lightsShader);

private:
	GLfloat mAspectRatio, mZNear, mZFar;
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;

	Camera *mCamera;

	Skybox *mSkybox;
	Shader *mSkyboxShader;

	std::vector<Entity*> mEntities;
	Shader *mEntityShader;
};