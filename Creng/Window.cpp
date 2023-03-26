#include "Window.h"

Window::Window() : Window(DEFAULT_WIDTH, DEFAULT_HEIGHT) {
}

Window::Window(GLint windowWidth, GLint windowHeight) {

	width = windowWidth;
	height = windowHeight;

	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMove = true;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

int Window::Initialise() {
	// Initialise GLFW
	if (!glfwInit()) {
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Creating a window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Handle Key + Mouse Input
	CreateCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	// Initialise GLEW
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Test to see which triangles are drawn on top of the others
	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}

GLfloat Window::GetBufferWidth() {
	return bufferWidth;
}

GLfloat Window::GetBufferHeight() {
	return bufferHeight;
}

bool Window::GetShouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

bool* Window::GetKeys() {
	return keys;
}

GLfloat Window::GetAndResetXChange() {
	GLfloat change = xChange;
	xChange = 0.0f;
	return change;
}

GLfloat Window::GetAndResetYChange() {
	GLfloat change = yChange;
	yChange = 0.0f;
	return change;
}

void Window::SwapBuffers() {
	glfwSwapBuffers(mainWindow);
}

void Window::HandleKeys(GLFWwindow* glfwWindow, int key, int code, int action, int mode) {

	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

		glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			window->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			window->keys[key] = false;
		}
	}
}

void Window::HandleMouse(GLFWwindow* glfwWindow, double xPos, double yPos) {

	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

	if (window->mouseFirstMove) {
		window->lastX = xPos;
		window->lastY = yPos;
		window->mouseFirstMove = false;
	}

	window->xChange = xPos - window->lastX;
	window->yChange = window->lastY - yPos;

	window->lastX = xPos;
	window->lastY = yPos;
}

void Window::CreateCallbacks() {
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
