#pragma once

#include <stdio.h>

#include <GL\glew.h>

class ShadowMap {

public:

	ShadowMap();
	~ShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);

	GLuint GetWidth();
	GLuint GetHeight();

private:

	GLuint FBO, shadowMap;
	GLuint width, height;

};

