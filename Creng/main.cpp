#define STB_IMAGE_IMPLEMENTATION

#include <cstdio>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

Camera* camera;
Window* mainWindow;
Texture* faceTexture;
Texture* skullTexture;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() {

	unsigned int indicies[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat verticies[] = {
	//	x	   y	  z			u	  v
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,		0.5f, 1.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,		0.5f, 0.0f
	};

	Mesh* mesh1 = new Mesh();
	mesh1->CreateMesh(verticies, indicies, 20, 12);
	meshList.push_back(mesh1);

	Mesh* mesh2 = new Mesh();
	mesh2->CreateMesh(verticies, indicies, 20, 12);
	meshList.push_back(mesh2);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

int main() {

	mainWindow = new Window(WIDTH, HEIGHT);
	mainWindow->Initialise();

	CreateObjects();
	CreateShaders();

	camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.3f);

	faceTexture = new Texture("Textures/Face.jpg");
	faceTexture->LoadTexture(); 
	skullTexture = new Texture("Textures/Skulls.jpg");
	skullTexture->LoadTexture();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	// Projection perspective
	glm::mat4 projection = glm::perspective(45.0f, mainWindow->GetBufferWidth() / mainWindow->GetBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow->GetShouldClose()) {

		// Calculating delta time
		GLfloat currTime = glfwGetTime();
		deltaTime = currTime - lastTime;
		lastTime = currTime;

		// Get + Handle user input events
		glfwPollEvents();

		// Getting the keys to move the camera
		camera->KeyControl(mainWindow->GetKeys(), deltaTime);
		camera->MouseControl(mainWindow->GetAndResetXChange(), mainWindow->GetAndResetYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw triangle
		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();

		glm::mat4 model(1.0f);
		// Order is IMPORTANT. If we do rotation or scale first, translation will be rotated or scaled too.
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->CalculateViewMatrix()));
		faceTexture->UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		skullTexture->UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);

		// Swaps buffer to the one that can be seen
		mainWindow->SwapBuffers();
	}

	return 0;
}