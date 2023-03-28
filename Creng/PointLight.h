#pragma once

#include "Light.h"

class PointLight : public Light {

public:

	PointLight();
	PointLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, 
		glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat exponent);

	~PointLight();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

private:

	glm::vec3 position;

	GLfloat constant, linear, exponent;

};

