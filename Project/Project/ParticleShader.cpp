#include "ParticleShader.h"

#include "Random.h"

bool ParticleShader::setupParticle = false;
GLuint ParticleShader::vbo_particle;
GLuint ParticleShader::ebo_particle;

ParticleShader::ParticleShader(const char* vertPath, const char* fragPath) : Shader(vertPath, fragPath) {
	if (!setupParticle) {
		makeParticles(); // do it only once
	}
}

void ParticleShader::makeParticles()
{
	// Each particle is a square with four vertices and two triangles

	// Number of attributes for vertices and faces
	const int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), direction (2), 2D texture coordinates (2), time (1)
								//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};

	GLfloat particleatt[4000 * vertex_attr];
	float displacement, delay, phase, animDelay;

	phase = 0.0;
	for (int i = 0; i < 4000; i++)
	{
		if (i % 4 == 0)
		{
			displacement = random::randomFloat(-0.4, 0.4);
			delay = random::randomFloat(-0.2, 0.2);
			animDelay = random::randomInt(1, 10);
			phase += 0.3;
		}
		// position	
		particleatt[i * vertex_attr + 0] = vertex[(i % 4) * 7 + 0];
		particleatt[i * vertex_attr + 1] = vertex[(i % 4) * 7 + 1];

		// displacement
		particleatt[i * vertex_attr + 2] = displacement;
		particleatt[i * vertex_attr + 3] = delay;

		// phase
		particleatt[i * vertex_attr + 4] = phase;

		// texture coordinate
		particleatt[i * vertex_attr + 5] = vertex[(i % 4) * 7 + 5];
		particleatt[i * vertex_attr + 6] = vertex[(i % 4) * 7 + 6];
	}


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint manyface[4000 * 6];

	for (int i = 0; i < 4000; i++)
	{
		for (int j = 0; j < 6; j++)
			manyface[i * 6 + j] = face[j] + i * 4;
	}

	// Create buffer for vertices
	glGenBuffers(1, &vbo_particle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_particle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleatt), particleatt, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo_particle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_particle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyface), manyface, GL_STATIC_DRAW);

	setupParticle = true;
}

void ParticleShader::setAttributes() {
	glBlendFunc(GL_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE); // draw particles without writing to depth buffer

	glBindBuffer(GL_ARRAY_BUFFER, vbo_particle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_particle);
	// Set attributes for shaders
	// Should be consistent with how we created the buffers for the particle elements
	GLint vertex_att = glGetAttribLocation(shaderProgram, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);

	GLint dir_att = glGetAttribLocation(shaderProgram, "dir");
	glVertexAttribPointer(dir_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(dir_att);

	GLint time_att = glGetAttribLocation(shaderProgram, "t");
	glVertexAttribPointer(time_att, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(time_att);

	GLint tex_att = glGetAttribLocation(shaderProgram, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);
}
