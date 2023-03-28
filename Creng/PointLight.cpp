#include "PointLight.h"

PointLight::PointLight() : PointLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f) {
}

PointLight::PointLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity,
	glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat exponent)
	: Light(color, ambientIntensity, diffuseIntensity) {

	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;
}

PointLight::~PointLight() {
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation,
	GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation) {

	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);

	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}
