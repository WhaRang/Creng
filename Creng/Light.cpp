#include "Light.h"

Light::Light() : Light(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f) {
}

Light::Light(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity) {

	this->color = color;
	this->ambientIntensity = ambientIntensity;
	this->diffuseIntensity = diffuseIntensity;
}

Light::~Light() {
}
