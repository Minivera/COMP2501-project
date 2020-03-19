#pragma once

#include "Shader.h"

class ParticleShader : public Shader
{
public:
	ParticleShader(const char* vertPath, const char* fragPath);

	virtual void setAttributes();

	void makeParticles();
private:
	static GLuint vbo_particle;
	static GLuint ebo_particle;

	static bool setupParticle;
};

