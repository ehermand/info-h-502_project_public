#pragma once

#include <GLFW/glfw3.h>

class Window {
public:
	Window(const char *title, int width, int height);
	~Window();

	void setCallbacks(GLFWkeyfun key_callback, GLFWcursorposfun mouse_callback, GLFWmousebuttonfun mouse_button_callback, GLFWscrollfun scroll_callback);
	void update();
	bool isClosed();

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }
private:
	const char *mTitle;
	int mWidth, mHeight;
	GLFWwindow *mWindow;

	void init();
};