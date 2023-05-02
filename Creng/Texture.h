#pragma once

#include <GL/glew.h>

#include "Resources.h"


class Texture {

public:

	Texture();
	Texture(const char* filePath);
	~Texture();

	bool LoadTexture(bool alpha);

	void ClearTexture();
	void UseTexture();

private:

	GLuint textureID;
	int width, height, bitDepth;

	const char* filePath;

};

