#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : 
	DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, glm::vec3(-1.0f, 0.0f, 0.0f)) {
}

DirectionalLight::DirectionalLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 direction)
	: Light(color, ambientIntensity, diffuseIntensity) {

	this->direction = direction;
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation) {

	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
