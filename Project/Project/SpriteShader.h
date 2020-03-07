#pragma once

#include "Shader.h"

class SpriteShader: public Shader
{
public:
	SpriteShader(const char* vertPath, const char* fragPath);

	virtual void setAttributes();

	void makeSprite();
private:
	static GLuint vbo_sprite;
	static GLuint ebo_sprite;

	static bool setupSprite;
};

