#pragma once

#include <GL\glew.h>

#include "stb_image.h"


class Texture {

public:

	Texture();
	Texture(const char* filePath);
	~Texture();

	void LoadTexture();
	void ClearTexture();
	void UseTexture();

private:

	GLuint textureID;
	int width, height, bitDepth;

	const char* filePath;

};

