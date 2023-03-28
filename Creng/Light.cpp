#include "Light.h"

Light::Light() : Light(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f) {
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseIntensity) {

	this->color = glm::vec3(red, green, blue);
	this->ambientIntensity = ambientIntensity;
	this->direction = glm::vec3(xDir, yDir, zDir);
	this->diffuseIntensity = diffuseIntensity;
}

Light::~Light() {
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation) {

	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
