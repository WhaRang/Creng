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

#include "Resources.h"

#include "Mesh.h" 
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"

const GLint WIDTH = 1920, HEIGHT = 1080;
const float toRadians = 3.14159265f / 180.0f;

Camera* camera;
Window* mainWindow;

DirectionalLight* mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Material* shinyMaterial;
Material* dullMaterial;

Texture* faceTexture;
Texture* skullTexture;
Texture* dirtTexture;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

void CalcAverageNormals(unsigned int* indicies, unsigned int indiceCount, GLfloat* verticies,
	unsigned int verticeCount, unsigned int vertexLenght, unsigned int normalOffset) {

	for (size_t i = 0; i < indiceCount; i += 3) {

		unsigned int in0 = indicies[i] * vertexLenght;
		unsigned int in1 = indicies[i + 1] * vertexLenght;
		unsigned int in2 = indicies[i + 2] * vertexLenght;

		glm::vec3 v1(verticies[in1] - verticies[in0], verticies[in1 + 1] - verticies[in0 + 1], verticies[in1 + 2] - verticies[in0 + 2]);
		glm::vec3 v2(verticies[in2] - verticies[in0], verticies[in2 + 1] - verticies[in0 + 1], verticies[in2 + 2] - verticies[in0 + 2]);

		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;

		verticies[in0] += normal.x; verticies[in0 + 1] += normal.y; verticies[in0 + 2] += normal.z;
		verticies[in1] += normal.x; verticies[in1 + 1] += normal.y; verticies[in1 + 2] += normal.z;
		verticies[in2] += normal.x; verticies[in2 + 1] += normal.y; verticies[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vertexLenght; i++) {

		unsigned int individualNormalOffset = i * vertexLenght + normalOffset;

		glm::vec3 normal(verticies[individualNormalOffset], verticies[individualNormalOffset + 1], verticies[individualNormalOffset + 2]);
		normal = glm::normalize(normal);

		verticies[individualNormalOffset] = normal.x;
		verticies[individualNormalOffset + 1] = normal.y;
		verticies[individualNormalOffset + 2] = normal.z;
	}
}

void CreateObjects() {

	unsigned int indicies[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat verticies[] = {
		//	 x	    y	  z			u	  v			nx    ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			 0.0f, -1.0f,  1.0f,	0.5f, 1.0f,		0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, -0.6f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			 0.0f,  1.0f,  0.0f,	0.5f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndicies[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVerticies[] = {
		-10.0f, 0.0f, -10.0f,	0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,   0.0f,  10.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,   10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	CalcAverageNormals(indicies, 12, verticies, 32, 8, 5);

	Mesh* pyramid1 = new Mesh();
	pyramid1->CreateMesh(verticies, indicies, 32, 12);
	meshList.push_back(pyramid1);

	Mesh* pyramid2 = new Mesh();
	pyramid2->CreateMesh(verticies, indicies, 32, 12);
	meshList.push_back(pyramid2);

	Mesh* floorMesh = new Mesh();
	floorMesh->CreateMesh(floorVerticies, floorIndicies, 32, 6);
	meshList.push_back(floorMesh);
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
	dirtTexture = new Texture("Textures/Dirt.jpg");
	dirtTexture->LoadTexture();

	shinyMaterial = new Material(4.0f, 256.0f);
	dullMaterial = new Material(1.0f, 4.0f);

	mainLight = new DirectionalLight(
		glm::vec3(1.0f, 1.0f, 1.0f),
		0.1f, 0.1f,
		glm::vec3(0.0f, 0.0f, -1.0f));

	unsigned int pointLightCount = 0;

	pointLights[pointLightCount] = PointLight(
		glm::vec3(0.0f, 0.0f, 1.0f),
		0.0f, 0.1f,
		glm::vec3(-4.0, 0.0f, 0.0f),
		0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	pointLights[pointLightCount] = PointLight(
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f, 0.1f,
		glm::vec3(4.0, 2.0f, 0.0f),
		0.3f, 0.1f, 0.1f);
	//pointLightCount++;

	unsigned int spotLightCount = 0;

	spotLights[0] = SpotLight(
		glm::vec3(1.0f, 1.0f, 1.0f),
		0.0f, 1.0f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(
		glm::vec3(1.0f, 1.0f, 1.0f),
		0.0f, 1.0f,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-100.0f, -1.0f, 0.0f),
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0;
	GLuint uniformSpecularIntensity = 0, uniformShininess = 0;

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

		shaderList[0]->UseShader();

		// World uniform values
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformEyePosition = shaderList[0]->GetEyePositionLocation();

		// Material uniform values
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
		uniformShininess = shaderList[0]->GetShininessLocation();

		glm::vec3 lowerLight = camera->GetPosition();
		lowerLight.y -= 0.2f;
		spotLights[0].SetFlash(lowerLight, camera->GetDirection());

		// Using the light
		shaderList[0]->SetDirectionalLight(mainLight);
		shaderList[0]->SetPointLights(pointLights, pointLightCount);
		shaderList[0]->SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->CalculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		faceTexture->UseTexture();
		shinyMaterial->UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		skullTexture->UseTexture();
		dullMaterial->UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture->UseTexture();
		dullMaterial->UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		glUseProgram(0);

		// Swaps buffer to the one that can be seen
		mainWindow->SwapBuffers();
	}

	return 0;
}