#pragma once

#include "Light.h"

class DirectionalLight : public Light {

public:

	DirectionalLight();
	DirectionalLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 direction);

	~DirectionalLight();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

private:

	glm::vec3 direction;
};

