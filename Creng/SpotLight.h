#pragma once

#include "PointLight.h"

class SpotLight : public PointLight {

public:

	SpotLight(); 
	SpotLight(
		GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat near, GLfloat far,
		glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity,
		glm::vec3 position, glm::vec3 direction,
		GLfloat constant, GLfloat linear, GLfloat exponent,
		GLfloat edge);

	~SpotLight();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	void SetFlash(glm::vec3 position, glm::vec3 direction);

	void Toggle();

private:

	glm::vec3 direction;

	GLfloat edge, cosEdge;

	bool isOn;

};

