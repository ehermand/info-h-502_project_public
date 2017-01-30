#include "Renderer.hpp"

#include <string>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 pointLights[] = {
	glm::vec3(0.96374f,3.5f,-10.0f),glm::vec3(0.96374f,3.5f,-5.0f),
	glm::vec3(0.96374f,3.5f,0.0f),glm::vec3(0.96374f,3.5f,10.0f) };

Renderer::Renderer() : mZNear(0.1f), mZFar(1000.0f) {
}

void Renderer::setCurrentCamera(Camera *camera) {
	mCamera = camera;
}
void Renderer::setSkybox(Skybox *skybox, Shader *skyboxShader) {
	mSkybox = skybox;
	mSkyboxShader = skyboxShader;
}
void Renderer::setEntityShader(Shader *shader) {
	mEntityShader = shader;
}
void Renderer::setAspectRatio(GLfloat aspectRatio) {
	mAspectRatio = aspectRatio;
}

void Renderer::clear() {
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f); // Background Fill Color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & z- buffers
}
void Renderer::updateMatrices() {
	mViewMatrix = mCamera->getViewMatrix();
	mProjectionMatrix = glm::perspective(glm::radians(mCamera->Zoom), mAspectRatio, mZNear, mZFar);
}

void Renderer::drawSkybox() {
	mSkyboxShader->use();
	mSkyboxShader->setInteger("skybox", 0);
	mSkyboxShader->setMatrix4("V", glm::mat4(glm::mat3(mViewMatrix)));
	mSkyboxShader->setMatrix4("P", mProjectionMatrix);
	mSkybox->draw();
}

void Renderer::drawEntity(Entity *entity) {
	mEntityShader->use();

	for (int i = 0; i < 4; i++) {
		std::string n = std::to_string(i);
		mEntityShader->setVector3f(("WlightPos["+n+"]").c_str(), pointLights[i]);
	}
	mEntityShader->setVector4f("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);

	mEntityShader->setVector3f("WeyePos", mCamera->Position);

	mEntityShader->setMatrix4("M", entity->getModelMatrix());
	mEntityShader->setMatrix4("V", mViewMatrix);
	mEntityShader->setMatrix4("P", mProjectionMatrix);
	mEntityShader->setMatrix4("MVP", mProjectionMatrix*mViewMatrix*(entity->getModelMatrix()));
	mEntityShader->setVector4f("someColor", 0.3f, 0.55f, 0.9f, 1.0f);
	entity->draw(*mEntityShader);
}

void Renderer::drawEntity(Entity *entity, Shader *shader) {
	shader->use();

	for (int i = 0; i < 4; i++) {
		std::string n = std::to_string(i);
		shader->setVector3f(("WlightPos[" + n + "]").c_str(), pointLights[i]);
	}
	shader->setVector4f("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);

	shader->setVector3f("WeyePos", mCamera->Position);

	shader->setMatrix4("M", entity->getModelMatrix());
	shader->setMatrix4("V", mViewMatrix);
	shader->setMatrix4("P", mProjectionMatrix);
	shader->setMatrix4("MVP", mProjectionMatrix*mViewMatrix*(entity->getModelMatrix()));
	shader->setVector4f("someColor", 0.3f, 0.55f, 0.9f, 1.0f);

	shader->setInteger("skybox", 3);
	mSkybox->bindTexture();

	entity->draw(*shader);
}

void Renderer::drawLights(Model *lightModel, Shader *lightsShader) {
	lightsShader->use();
	for (int i = 0; i < 4; i++) {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, pointLights[i]);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));

		lightsShader->setMatrix4("MVP", mProjectionMatrix*mViewMatrix*modelMatrix);
		lightsShader->setVector4f("someColor", 1.0f, 1.0f, 1.0f, 1.0f);

		lightModel->draw(*lightsShader);
	}
}