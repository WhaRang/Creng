#include "Texture.h"

Texture::Texture() : Texture("") {
}

Texture::Texture(const char* filePath) {
	this->textureID = 0;
	this->width = 0;
	this->height = 0;
	this->bitDepth = 0;
	this->filePath = filePath;
}

Texture::~Texture() {
	ClearTexture();
}

bool Texture::LoadTexture(bool alpha) {

	unsigned char* textureData = stbi_load(filePath, &width, &height, &bitDepth, 0);

	if (!textureData) {
		printf("Failed to find: %s\n", filePath);
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Wrapping the texture when going outside of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Closer/further from the image
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// depending on demand, aplha channel will be included
	GLuint format = alpha ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	return true;
}

void Texture::ClearTexture() {

	glDeleteTextures(1, &textureID);
	
	this->textureID = 0;
	this->width = 0;
	this->height = 0;
	this->bitDepth = 0;
	this->filePath = "";
}

void Texture::UseTexture() {

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);
}
