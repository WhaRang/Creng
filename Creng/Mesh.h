#pragma once

#include <GL\glew.h>

class Mesh {

public:

	Mesh();
	~Mesh();

	void CreateMesh(GLfloat *verticies, unsigned int *indicies, unsigned int verticiesNum, unsigned int indiciesNum);
	void RenderMesh();
	void ClearMesh();

private:

	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

};