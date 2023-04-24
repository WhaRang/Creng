#pragma once

#include "ShadowMap.h"

class OmniShadowMap : public ShadowMap {

public:

	OmniShadowMap();
	~OmniShadowMap();

	bool Init(unsigned int width, unsigned int height);

	void Write();
	void Read(GLenum textureUnit);

};

