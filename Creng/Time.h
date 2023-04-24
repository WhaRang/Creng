#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Time {

public:
	
	static void StartUpdate();
	static void StopUpdate();

	static GLfloat deltaTime;

private:

	static GLfloat start;
	static GLfloat end;

};

