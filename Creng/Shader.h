#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include "Resources.h"

#include "DirectionalLight.h"
#include "SpotLight.h"

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
	void SetSpotLights(SpotLight* light, unsigned int lightCount);

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

	struct {
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint position;
		GLuint direction;
		GLuint constant;
		GLuint linear;
		GLuint exponent;
		GLuint edge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	int pointLightCount, spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition;
	GLuint uniformSpecularIntensity, uniformShininess;
	GLuint uniformPointLightCount, uniformSpotLightCount;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileTransform();
	void CompileDirectionalLight();
	void CompileMaterials();
	void CompilePointLight();
	void CompileSpotLight();

	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);

	std::string ReadFile(const char* path);
};