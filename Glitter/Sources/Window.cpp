#include "Window.hpp"

#include <glad/glad.h>
#include <iostream>

Window::Window(const char *title, int width, int height) {
	mTitle = title;
	mWidth = width;
	mHeight = height;

	init();
}

Window::~Window() {
	glfwTerminate();
}

void Window::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, nullptr, nullptr);

	// Check for valid context
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
	}

	// Create context
	glfwMakeContextCurrent(mWindow);

	// Load OpenGL Functions
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// Change viewport
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	glViewport(0, 0, width, height);

	// Set cursor settings
	glfwSetCursorPos(mWindow,width/2,height/2); //Move cursor to the center
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set OpenGL options
	glEnable(GL_DEPTH_TEST); // Enable z-buffer
	glEnable(GL_CULL_FACE); // Enable face culling
}

void Window::setCallbacks(GLFWkeyfun key_callback, GLFWcursorposfun mouse_callback, GLFWmousebuttonfun mouse_button_callback, GLFWscrollfun scroll_callback) {
	// Set the required callback functions
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
	glfwSetScrollCallback(mWindow, scroll_callback);
}

void Window::update() {
	// Flip Buffers and Draw
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

bool Window::isClosed() {
	return glfwWindowShouldClose(mWindow);
}
