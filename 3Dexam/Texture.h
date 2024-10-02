#pragma once
#include "stb/stb_image.h"
#include"Resources/Shaders/shaderClass.h"
class Texture
{
	int ID; 
public:
	unsigned int texture;

	Texture(const char* texture1, Shader shaderProgram);

	
};

