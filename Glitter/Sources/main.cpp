// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "Window.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"

#include "Entity.hpp"
#include "BallEntity.hpp"
#include "PinEntity.hpp"

#include "PhysicsEngine.hpp"


// Callbacks
static bool keys[1024]; // is a key pressed or not ?
						// External static callback
						// Is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/);
static void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset);

const float a = 0.1f;
const float b = a*sqrt(3);
const glm::vec3 relativePositions[10] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(-a, 0.0f, -b), glm::vec3(+a, 0.0f, -b),
	glm::vec3(-2 * a, 0.0f, -2 * b), glm::vec3(0.0f, 0.0f, -2 * b), glm::vec3(2 * a, 0.0f, -2 * b),
	glm::vec3(-3 * a, 0.0f, -3 * b), glm::vec3(-a, 0.0f, -3 * b), glm::vec3(a, 0.0f, -3 * b), glm::vec3(3 * a, 0.0f, -3 * b)
};

float lastX;
float lastY;
bool firstIt = true;
double movLastTime = 0.0f;
float movDeltaTime = 0.0f;
void moveCamera();

void showFPS(void);
float deltaTime = 0.0f;

bool reset = false;
bool launchBall = false;

// Create camera
Camera camera(glm::vec3(0.0f, 1.0f, 10.0f));

int main(int argc, char * argv[]) {

    // Setup window
	Window window("INFO-H-502 Project - Underwater Bowling", 1024, 768);
	window.setCallbacks(key_callback, mouse_callback, mouse_button_callback, scroll_callback);

	// Setup renderer
	Renderer renderer;
	renderer.setAspectRatio((float)window.getWidth()/(float)window.getHeight());

    renderer.setCurrentCamera(&camera);

	// Set physics engine
	PhysicsEngine physicsEngine;
	physicsEngine.addGround();
	physicsEngine.addLane();

	// Create skybox
	Shader skyboxShader("./Glitter/Shaders/skybox.vert", "./Glitter/Shaders/skybox.frag");
	skyboxShader.compile();
	Skybox skybox("./Glitter/mp_whirlpool/", "tga");

	renderer.setSkybox(&skybox, &skyboxShader);

	// Create and compile shader programs
	Shader triangleShader("./Glitter/Shaders/triangle.vert","./Glitter/Shaders/triangle.frag");
	triangleShader.compile();

    Shader lightingShader("./Glitter/Shaders/lighting.vert","./Glitter/Shaders/lighting.frag");
    lightingShader.compile();

    Shader normalShader("./Glitter/Shaders/normal.vert","./Glitter/Shaders/normal.frag","./Glitter/Shaders/normal.geom");
    normalShader.compile();

    Shader normalColorShader("./Glitter/Shaders/normalColor.vert","./Glitter/Shaders/normalColor.frag");
    normalColorShader.compile();

    Shader reflectionShader("./Glitter/Shaders/reflection.vert","./Glitter/Shaders/reflection.frag");
    reflectionShader.compile();

    Shader refractionShader("./Glitter/Shaders/refraction.vert","./Glitter/Shaders/refraction.frag");
    refractionShader.compile();

	Shader entityShader("./Glitter/Shaders/entity.vert", "./Glitter/Shaders/entity.frag");
	entityShader.compile();

	Shader lightsShader("./Glitter/Shaders/lightsShader.vert", "./Glitter/Shaders/lightsShader.frag");
	lightsShader.compile();

	// Load models
	Model ballModel("./Glitter/bowling_ball/bowling_ball.obj");
	Model pinModel("./Glitter/bowling_pin/bowling_pin.obj");
	Model glassModel("./Glitter/glass/half-cylinder_closed.obj");
	Model laneModel("./Glitter/bowling_lane/bowling_lane.obj");
	Model lightModel("./Glitter/sphere.obj");

	// Create entities
	BallEntity ball(&ballModel, glm::vec3(0.06f, 0.5f, 10.0f), glm::vec3(0.0f), glm::vec3(0.001f));
	Entity glass(&glassModel, glm::vec3(0.96374f, 0.0f, -10.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	Entity lane(&laneModel, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	physicsEngine.addBall(&ball);

	std::vector<PinEntity> pins;

	// First pin set
	for (int i = 0; i < 10; i++) {
		PinEntity pin(&pinModel, glm::vec3(0.0f, 0.1f, -9.5f) + relativePositions[i], glm::vec3(0.0f), glm::vec3(0.001f));
		pins.push_back(pin);
	}
	// Second pin set
	for (int i = 0; i < 10; i++) {
		PinEntity pin(&pinModel, glm::vec3(2*0.96374f, 0.0f, -9.5f) + relativePositions[i], glm::vec3(0.0f), glm::vec3(0.001f));
		pins.push_back(pin);
	}
	// Add only the left lane pins to the physics world
	for (int i = 0; i < 10; i++) {
		physicsEngine.addBowlingPin(&(pins[i]));
	}

	renderer.setEntityShader(&entityShader);


    // Rendering Loop
    while (!window.isClosed()) {
		showFPS();

		// Physics update
		if (reset) {
			physicsEngine.resetDefaultState();
			reset = false;
		}
		if(launchBall) {
			physicsEngine.launchBall(80.0f,camera.Position,camera.Front); //glm::vec3(0.06f, 0.5f, 10.0f),glm::vec3(0, 0, -1.0f)
			launchBall = false;
		}

		physicsEngine.update(movDeltaTime/2);

		// Camera movement
		moveCamera();

		if (camera.isAttached)
			camera.Position = ball.getPosition();
		
		// Render world
		renderer.clear();
		renderer.updateMatrices();

		renderer.drawLights(&lightModel,&lightsShader);

		renderer.drawEntity(&glass, &refractionShader);

		renderer.drawEntity(&lane);

		for (int i = 0; i < pins.size(); i++) {
			renderer.drawEntity(&pins[i]);
		}

		renderer.drawEntity(&ball, &reflectionShader);

		window.update();
    }
	//system("PAUSE");
    return EXIT_SUCCESS;
}

void showFPS(void) {
	static double lastTime = glfwGetTime();
	static int nbFrames = 0;

	// Measure speed
	double currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last printf() was more than 1 sec ago
										 // printf and reset timer
		std::cout << 1000.0 / double(nbFrames) << " ms/frame -> " << nbFrames << " frames/sec" << std::endl;
		nbFrames = 0;
		lastTime += 1.0;
	}
}


static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	// V-SYNC
	if (keys[GLFW_KEY_U]) {
		static bool vsync = true;
		if (vsync) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		vsync = !vsync;
	}

	// Toggle freemove mode
	if (keys[GLFW_KEY_F]) {
	    static bool freemove = true;
	    if (freemove) {
	        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	    }
	    else {
	        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	    }
	    freemove = !freemove;
	}
	// Attach camera to ball
	if (keys[GLFW_KEY_Z]) {
		camera.isAttached = !camera.isAttached;
	}
	// Reset physics world
	if (keys[GLFW_KEY_R]) {
		reset = true;
	}
	// Launch ball
	if (keys[GLFW_KEY_T]) {
		launchBall = true;
	}
}
void moveCamera() {
    // Camera movement
    movDeltaTime = glfwGetTime() - movLastTime;
    movLastTime = glfwGetTime();
    if (keys[GLFW_KEY_W]) {
        camera.processKeyboard(FORWARD,movDeltaTime);
    }
    if (keys[GLFW_KEY_S]) {
        camera.processKeyboard(BACKWARD,movDeltaTime);
    }
    if (keys[GLFW_KEY_A]) {
        camera.processKeyboard(LEFT,movDeltaTime);
    }
    if (keys[GLFW_KEY_D]) {
        camera.processKeyboard(RIGHT,movDeltaTime);
    }
}

static void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		keys[GLFW_MOUSE_BUTTON_RIGHT] = true;
	else
		keys[GLFW_MOUSE_BUTTON_RIGHT] = false;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		keys[GLFW_MOUSE_BUTTON_LEFT] = true;
	else
		keys[GLFW_MOUSE_BUTTON_LEFT] = false;

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		keys[GLFW_MOUSE_BUTTON_MIDDLE] = true;
	else
		keys[GLFW_MOUSE_BUTTON_MIDDLE] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Camera rotation
    if (firstIt) {
        lastX = xpos;
        lastY = ypos;
        firstIt = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    if (keys[GLFW_MOUSE_BUTTON_LEFT]) {
        camera.processMouseMovement(xoffset,yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
	// Camera zoom
	camera.processMouseScroll(yoffset);
}
