#include "Material.h"

Material::Material() : Material(0.0f, 0.0f) {
}

Material::Material(GLfloat specularIntensity, GLfloat shininess) {
	this->specularIntensity = specularIntensity;
	this->shininess = shininess;
}

Material::~Material() {
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) {

	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}
