#include "LaserShader.h"

bool LaserShader::setupSprite = false;
GLuint LaserShader::vbo_laser;
GLuint LaserShader::ebo_laser;

LaserShader::LaserShader(const char* vertPath, const char* fragPath) : Shader(vertPath, fragPath) {
	if (!setupSprite) {
		makeParticles(); // do it only once
	}
}

void LaserShader::makeParticles()
{
	// Each particle is a square with four vertices and two triangles

	// Number of attributes for vertices and faces
	const int vertex_attr = 2;  // 2 attributes per vertex: 2D (or 3D) position (2)
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

	for (int i = 0; i < 4000; i++)
	{
		// position	
		particleatt[i * vertex_attr + 0] = vertex[(i % 4) * 7 + 0];
		particleatt[i * vertex_attr + 1] = vertex[(i % 4) * 7 + 1];
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
	glGenBuffers(1, &vbo_laser);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_laser);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleatt), particleatt, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo_laser);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_laser);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyface), manyface, GL_STATIC_DRAW);

	setupSprite = true;
}

void LaserShader::setAttributes() {
	glBlendFunc(GL_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE); // draw particles without writing to depth buffer

	glBindBuffer(GL_ARRAY_BUFFER, vbo_laser);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_laser);

	// Set attributes for shaders
	// Should be consistent with how we created the buffers for the particle elements
	GLint vertex_att = glGetAttribLocation(shaderProgram, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);
}