#include "Shader.h"

Shader::Shader() {

	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

Shader::~Shader() {
	ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromString(const char* vertexCode, const char* geometryCode, const char* fragmentCode) {
	CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexPath, const char* fragmentPath) {

	std::string vertexString = ReadFile(vertexPath);
	std::string fragmentString = ReadFile(fragmentPath);

	const char* vertextCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CreateFromString(vertextCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexPath, const char* geometryPath, const char* fragmentPath) {

	std::string vertexString = ReadFile(vertexPath);
	std::string geometryString = ReadFile(geometryPath);
	std::string fragmentString = ReadFile(fragmentPath);

	const char* vertextCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CreateFromString(vertextCode, geometryCode, fragmentCode);
}

std::string Shader::ReadFile(const char* path) {

	std::string content;
	std::ifstream fileStream(path, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", path);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::UseShader() {
	glUseProgram(shaderID);
}

void Shader::ClearShader() {

	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

GLuint Shader::GetProjectionLocation() {
	return uniformProjection;
}

GLuint Shader::GetModelLocation() {
	return uniformModel;
}

GLuint Shader::GetViewLocation() {
	return uniformView;
}

GLuint Shader::GetEyePositionLocation() {
	return uniformEyePosition;
}

GLuint Shader::GetAmbientIntensityLocation() {
	return uniformDirectionalLight.ambientIntensity;
}

GLuint Shader::GetColorLocation() {
	return uniformDirectionalLight.color;
}

GLuint Shader::GetDiffuseIntensityLocation() {
	return uniformDirectionalLight.diffuseIntensity;
}

GLuint Shader::GetDirectionLocation() {
	return uniformDirectionalLight.direction;
}

GLuint Shader::GetSpecularIntensityLocation() {
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation() {
	return uniformShininess;
}

GLuint Shader::GetOmniLightPosLocation() {
	return uniformOmniLightPos;
}

GLuint Shader::GetFarPlaneLocation() {
	return uniformFarPlane;
}

void Shader::SetDirectionalLight(DirectionalLight* light) {
	light->UseLight(uniformDirectionalLight.ambientIntensity, uniformDirectionalLight.color,
		uniformDirectionalLight.diffuseIntensity, uniformDirectionalLight.direction);
}

void Shader::SetPointLights(PointLight* light, unsigned int lightCount) {

	if (lightCount > MAX_POINT_LIGHTS) {
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		light[i].UseLight(uniformPointLight[i].ambientIntensity, uniformPointLight[i].color,
			uniformPointLight[i].diffuseIntensity, uniformPointLight[i].position,
			uniformPointLight[i].constant, uniformPointLight[i].linear, uniformPointLight[i].exponent);
	}
}

void Shader::SetSpotLights(SpotLight* light, unsigned int lightCount) {

	if (lightCount > MAX_SPOT_LIGHTS) {
		lightCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		light[i].UseLight(uniformSpotLight[i].ambientIntensity, 
			uniformSpotLight[i].color, uniformSpotLight[i].diffuseIntensity,
			uniformSpotLight[i].position, uniformSpotLight[i].direction,
			uniformSpotLight[i].constant, uniformSpotLight[i].linear, uniformSpotLight[i].exponent,
			uniformSpotLight[i].edge);
	}
}

void Shader::SetTextureUnit(GLuint textureUnit) {
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit) {
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* transform) {
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*transform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices) {
	for (size_t i = 0; i < 6; i++) {
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType) {

	GLuint newShader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(newShader, 1, code, codeLength);
	glCompileShader(newShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(newShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(newShader, sizeof(eLog), NULL, eLog);
		printf("Error compile the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(program, newShader);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {

	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode) {

	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileProgram()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	CompileTransform();
	CompileDirectionalLight();
	CompileMaterials();
	CompilePointLight();
	CompileSpotLight();
	CompileShadow();
}

void Shader::CompileTransform() {

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
}

void Shader::CompileDirectionalLight() {

	uniformDirectionalLight.color = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.ambientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.diffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.direction = glGetUniformLocation(shaderID, "directionalLight.direction");
}

void Shader::CompileMaterials() {
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
}

void Shader::CompilePointLight() {

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {

		char locBuff[128] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLight[i].color = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].ambientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].diffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].position = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].constant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].linear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].exponent = glGetUniformLocation(shaderID, locBuff);
	}
}

void Shader::CompileSpotLight() {

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {

		char locBuff[128] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].color = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].ambientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].diffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].position = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].constant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].linear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].exponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].direction = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].edge = glGetUniformLocation(shaderID, locBuff);
	}
}

void Shader::CompileShadow() {

	uniformTexture = glGetUniformLocation(shaderID, "texSamp");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "dirLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "dirShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; i++) {

		char locBuff[128] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}
}