#include "SpotLight.h"

SpotLight::SpotLight() : SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 
	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f, 1.0f, 0.0f) {
}

SpotLight::SpotLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, 
	glm::vec3 position, glm::vec3 direction, 
	GLfloat constant, GLfloat linear, GLfloat exponent,
	GLfloat edge) : PointLight(color, ambientIntensity, diffuseIntensity, position, constant, linear, exponent) {

	this->direction = glm::normalize(direction);
	this->edge = edge;

	this->cosEdge = cosf(glm::radians(edge));
}

SpotLight::~SpotLight() {
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, 
	GLuint positionLocation, GLuint directionLocation, GLuint constantLocation, GLuint linearLocation, 
	GLuint exponentLocation, GLuint edgeLocation) {

	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);

	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, cosEdge);
}

void SpotLight::SetFlash(glm::vec3 position, glm::vec3 direction) {
	this->position = position;
	this->direction = direction;
}
