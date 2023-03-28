#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

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
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();

private:

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition;
	GLuint uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection;
	GLuint uniformSpecularIntensity, uniformShininess;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);

	std::string ReadFile(const char* path);
};