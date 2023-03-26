#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window {

public:

	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int Initialise();

	GLfloat GetBufferWidth();
	GLfloat GetBufferHeight();

	bool GetShouldClose();
	bool* GetKeys();
	GLfloat GetAndResetXChange();
	GLfloat GetAndResetYChange();

	void SwapBuffers();

private:

	static void HandleKeys(GLFWwindow* glfwWindow, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* glfwWindow, double xPos, double yPos);

	void CreateCallbacks();

	const float DEFAULT_WIDTH = 800, DEFAULT_HEIGHT = 600;

	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	GLfloat lastX, lastY;
	GLfloat xChange, yChange;

	bool mouseFirstMove;

	bool keys[1024];

};