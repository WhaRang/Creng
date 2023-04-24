#include "Time.h"

float Time::deltaTime = 0.0f;

float Time::start = 0.0f;

float Time::end = 0.0f;

void Time::StartUpdate() {
	start = glfwGetTime();
}

void Time::StopUpdate() {

	end = glfwGetTime();
	deltaTime = end - start;
}
