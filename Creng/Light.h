#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

class Light {

public:

	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight, glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity);

	~Light();

	ShadowMap* GetShadowMap();

protected:

	glm::vec3 color;
	glm::mat4 lightProj;

	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	ShadowMap* shadowMap;
};

