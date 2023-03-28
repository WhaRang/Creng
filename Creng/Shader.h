#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include "Resources.h"

#include "DirectionalLight.h"
#include "PointLight.h"

class Shader {

public:

	Shader();
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexPath, const char* fragmentPath);

	void UseShader();
	void ClearShader();

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetEyePositionLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();

	void SetDirectionalLight(DirectionalLight* light);
	void SetPointLights(PointLight* light, unsigned int lightCount);

private:

	struct {
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint direction;
	} uniformDirectionalLight;

	struct {
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint position;
		GLuint constant;
		GLuint linear;
		GLuint exponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	int pointLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition;
	GLuint uniformSpecularIntensity, uniformShininess;
	GLuint uniformPointLightCount;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);

	std::string ReadFile(const char* path);
};