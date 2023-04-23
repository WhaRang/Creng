#include "Light.h"

Light::Light() {
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);
	this->ambientIntensity = 1.0f;
	this->diffuseIntensity = 0.0f;
}

Light::Light(GLfloat shadowWidth, GLfloat shadowHeight, glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity) {

	this->shadowMap = new ShadowMap();
	this->shadowMap->Init(shadowWidth, shadowHeight);

	this->color = color;
	this->ambientIntensity = ambientIntensity;
	this->diffuseIntensity = diffuseIntensity;
}

Light::~Light() {
}

ShadowMap* Light::GetShadowMap() {
	return shadowMap;
}
