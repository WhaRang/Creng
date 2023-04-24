#pragma once

#include <vector>
#include "Light.h"
#include "OmniShadowMap.h"

class PointLight : public Light {

public:

	PointLight();
	PointLight(
		GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat near, GLfloat far,
		glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity,
		glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat exponent);

	~PointLight();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();

	GLfloat GetFarPlane();
	glm::vec3 GetPosition();

protected:

	glm::vec3 position;

	GLfloat constant, linear, exponent;

	GLfloat farPlane;


};

