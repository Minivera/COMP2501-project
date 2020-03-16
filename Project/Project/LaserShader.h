#pragma once

#include "Shader.h"

class LaserShader : public Shader
{
public:
	LaserShader(const char* vertPath, const char* fragPath);

	virtual void setAttributes();

	void makeParticles();
private:
	static GLuint vbo_laser;
	static GLuint ebo_laser;

	static bool setupSprite;
};
