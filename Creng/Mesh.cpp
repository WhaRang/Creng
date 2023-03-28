#include "Mesh.h"

Mesh::Mesh() {

	VAO = 0;
	VBO = 0;
	IBO = 0;

	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* verticies, unsigned int* indicies, unsigned int verticiesCount, unsigned int indiciesCount) {

	indexCount = indiciesCount;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies[0]) * indiciesCount, indicies, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies[0]) * verticiesCount, verticies, GL_STATIC_DRAW);

	// Getting vertex position to vertex shader throught location = 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(verticies[0]) * 8, 0);
	glEnableVertexAttribArray(0);

	// Getting u and v for texture to vertex shader throught location = 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(verticies[0]) * 8, (void*)(sizeof(verticies[0]) * 3));
	glEnableVertexAttribArray(1);

	// Getting normals to vertex shader throught location = 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(verticies[0]) * 8, (void*)(sizeof(verticies[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh() {

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh() {
	
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh() {
	ClearMesh();
}
