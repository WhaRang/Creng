#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Resources.h"
#include "Shader.h"
#include "Mesh.h"

class Skybox {

public:

	Skybox();
	Skybox(std::vector<std::string> faceLocations);

	void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~Skybox();

private:

	const char* VERT_SHADER_LOCATION = "Shaders/skybox.vert";
	const char* FRAG_SHADER_LOCATION = "Shaders/skybox.frag";

	Mesh* mesh;
	Shader* shader;

	GLuint textureId;
	GLuint uniformProjection, uniformView;

};